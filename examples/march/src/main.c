#include "bpaint/formats/bmp.h"
#include <bpaint/alias.h>
#include <bpaint/bpaint.h>
#include <bpaint/canvas.h>
#include <bpaint/hash.h>
#include <bpaint/noise.h>
#include <bpaint/sdf.h>
#include <bpaint/utils.h>
#include <bpaint/v3.h>
#include <math.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define MAX_BUFFER 98192

static int serve(uint8_t* data, uint64_t size) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    // Construct an HTTP response
    char response[MAX_BUFFER];
    sprintf(response, 
            "HTTP/1.1 200 OK\n"
            "Content-Type: image/bmp\n"
            "Content-Length: %zu\n\n", size);

    // Send the headers first
    send(new_socket, response, strlen(response), 0);

    // Then send the image data
    send(new_socket, data, size, 0);

    clock_t before = clock();

    clock_t now = clock();

    double diff = (now - before) / (double) CLOCKS_PER_SEC * 1000.0f;

    while (diff < 2000.0f) {
      now = clock();
      diff = (now - before) / (double) CLOCKS_PER_SEC * 1000.0f;
    }


    close(new_socket);

    return 0;
}


#define MARCH_STEPS 100
#define MARCH_NEAR 0.001f
#define MARCH_FAR 100.0f

typedef struct {
  float d;
  BPV3 n;
  BPV3 p;
  bool is_box;
} MarchData;

static float mysdf(BPV3 p, MarchData *data) {
  BPV3 n = bpaint_noise23(V2SCALE(V2(p.x, p.z), 0.7f), 34.492981f);

  float h = (n.x + n.y * n.z) * 0.5f;

  float ground = p.y - (0.02f + h);
  float box =
      bpaint_sdf_box(V3SUB(p, V3(0, 1, 4)), V3(1.0f, 1.0f, 1.0f)) - 0.15f;

  if (box < ground)
    data->is_box = true;
  else
    data->is_box = false;

  return fminf(ground, box);
}

static bool march(BPV3 ro, BPV3 rd, MarchData *data) {
  float d = 0.0f;

  for (int i = 0; i < MARCH_STEPS; i++) {
    BPV3 p = V3ADD(ro, V3SCALE(rd, d));

    float next = mysdf(p, data);
    d += next;

    if (fabsf(next) <= MARCH_NEAR)
      break;
    if (fabsf(d) >= MARCH_FAR)
      return false;
  }

  BPV3 p = V3ADD(ro, V3SCALE(rd, d));
  d = fabsf(d);
  data->d = d;
  data->p = p;
  BPV2 e = V2(0.0001f, 0.0f);
  float dd = mysdf(p, data);
  data->n = bpaint_v3_unit(
      V3SUB(V3(dd, dd, dd), V3(mysdf(V3SUB(p, V3(e.x, e.y, e.y)), data),
                               mysdf(V3SUB(p, V3(e.y, e.x, e.y)), data),
                               mysdf(V3SUB(p, V3(e.y, e.y, e.x)), data))));

  return true;
}

static BPV2 get_uv(BPV3 p, BPV3 n) {
  return V2MIX(
      V2MIX(V2(p.x, p.y), V2(p.y, p.z), roundf(V3ADOT(n, V3(1, 0, 0)))),
      V2(p.x, p.z), roundf(V3ADOT(n, V3(0, 1, 0))));
}

static BPV3 grass(BPV2 uv, float seed) {
  BPV3 n = bpaint_noise23_layered(V2SCALE(uv, 5.0f), 32.291921f, 9.3f, 3);
  float n1 = bpaint_noise21_layered(V2SCALE(uv, 4.0f), seed, 6.5f, 6);

  n1 = bpaint_smoothstep(0.4f, 0.7f, n1);

  BPV3 g1 = V3(0.15f, 0.8f, 0.1f);
  BPV3 g2 = V3(0.25f, 0.9f, 0.15f);
  BPV3 g3 = V3(0.35f, 0.7f, 0.19f);
  BPV3 g = V3MIX(V3MIX(g1, g2, n.x), g3, n.y);

  g = V3MIX(g, V3MUL(g, g), n1);

  return g;
}

static BPV3 pattern(BPV2 uv, float seed) {
  float n = bpaint_noise21_layered(V2SCALE(uv, 8.0f), seed, 0.2f, 3);
  return V3(n, n, n);
}

static BPV4 my_shade_func(uint32_t idx, BPV2 fc, BPV2 res) {
  float ux = (fc.x - 0.5f * res.x) / res.y;
  float uy = (fc.y - 0.5f * res.y) / res.y;
  BPV2 uv = V2(ux, uy);
  BPV3 rd = bpaint_v3_unit(V3(uv.x, uv.y, 1.0f));
  BPV3 col = V3(0, 0, 0);

  rd = bpaint_v3_rotate(rd, V3(0, 1, 0), bpaint_radians(16.0f));
  BPV3 ro = V3(0, 1.0f, -0.5f);
  MarchData data = {0};
  if (march(ro, rd, &data)) {
    BPV2 hituv = get_uv(data.p, data.n);
    hituv = V2SCALE(hituv, 4.0f);
    BPV3 L = bpaint_v3_unit(V3(1, 2, -3));
    BPV3 light_color = V3(0.96f, 0.79f, 0.69f);
    BPV3 diffuse =
        data.is_box ? pattern(hituv, 33.39281f) : grass(hituv, 3.2891822f);
    diffuse = V3SCALE(diffuse, 1.0f / M_PI);
    BPV3 ref = bpaint_v3_reflect(L, data.n);
    float VdotR = fmaxf(0.0f, bpaint_v3_dot(rd, ref));
    float spec = powf(VdotR, 32.0f);
    float dp = fmaxf(0.0f, bpaint_v3_dot(data.n, L));
    col = bpaint_v3_add_f(V3MUL(V3MUL(V3(dp, dp, dp), diffuse), light_color),
                          spec);
  } else {
    col = V3(0.29, 0.61, 0.9);
  }

  col = bpaint_tonemap_aces(col);
  col = bpaint_v3_pow_f(col, 1.0f / 2.2f);
  return V4(col.x, col.y, col.z, 1.0f);
}

int main(int argc, char *argv[]) {
  int w = 720;
  int h = w / 16 * 9;

  // create a new canvas.
  BPaintCanvas canvas = {0};
  bpaint_canvas_create(&canvas, (BPaintCanvasConfig){.width = w, .height = h});

  // fill canvas with some color.
  bpaint_canvas_fill(&canvas,
                     (BPV4){.x = 0.5f, .y = 0.48f, .z = 0.48f, .w = 1.0f});

  // "shade" the canvas using mutliple threads.
  bpaint_canvas_shade(canvas, (BPaintShadeDispatchConfig){

                                  .func = my_shade_func, // Your shade function

                                  .n_x = 8, // N threads in the X-axis

                                  .n_y = 8 // N threads in the Y-axis
                              });

  /*  uint8_t* tmp = (uint8_t*)calloc(canvas.size_bytes, sizeof(uint8_t));
  bpaint_write_to_buffer__bmp(&canvas.data[0], canvas.config.width, canvas.config.height, &tmp[0]);
  serve(tmp, (uint64_t)canvas.size_bytes);
  free(tmp);
  tmp = 0;*/
  // Write the bitmap to disk.
  FILE *fp = fopen("image.bmp", "wb+");
  bpaint_canvas_write_to_disk(canvas, fp, BPAINT_FORMAT_BMP);
  fclose(fp);

  // Destroy canvas.
  bpaint_canvas_destroy(&canvas);
  return 0;
}





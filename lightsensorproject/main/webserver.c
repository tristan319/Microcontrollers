#include "webserver.h"
#include "esp_http_server.h"
#include "myADC.h"
#include <stdio.h>

static int current_channel;

extern const char index_html_start[] asm("_binary_index_html_start");
extern const char index_html_end[] asm("_binary_index_html_end");

static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_send(req, index_html_start,
                    index_html_end - index_html_start);
    return ESP_OK;
}

static esp_err_t light_get_handler(httpd_req_t *req)
{
    int value = myADC_getValue(current_channel);

    char resp[32];
    sprintf(resp, "%d", value);

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_webserver(int adc_channel)
{
    current_channel = adc_channel;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    httpd_start(&server, &config);

    httpd_uri_t root = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_get_handler};

    httpd_uri_t light = {
        .uri = "/light",
        .method = HTTP_GET,
        .handler = light_get_handler};

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &light);
}
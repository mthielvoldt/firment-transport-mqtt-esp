#ifndef ESP_ERR_H
#define ESP_ERR_H

typedef int esp_err_t;

#define ESP_OK 0

#define ESP_ERROR_CHECK(x) \
  do                       \
  {                        \
    if (x != ESP_OK)       \
      return false;        \
  } while (0)

#endif
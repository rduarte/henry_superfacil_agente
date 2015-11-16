#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <curl/curl.h> /* libcurl (http://curl.haxx.se/libcurl/c) */
#include <json-c/json.h> /* json-c (https://github.com/json-c/json-c) */

#define PACKAGE    "henry-superfacil-agent"
#define VERSION    "0.0.1"

struct MemoryStruct {
  char *memory;
  size_t size;
};

struct curl_fetch_st {
    char *payload;
    size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
 
int main(void)
{
  CURL *curl_handle;
  CURLcode res;
  CURLcode rcode; 
  json_object *json;

  enum json_tokener_error jerr = json_tokener_success;

  struct curl_slist *headers = NULL;
  struct curl_fetch_st curl_fetch; 
  struct curl_fetch_st *cf = &curl_fetch;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;
 
  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://192.168.10.223/rep.html?pgCode=7&opType=1&lblId=0&lblLogin=rep&lblPass=362190");
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  res = curl_easy_perform(curl_handle);
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    return 0;
  }

  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://192.168.10.223/rep.html?pgCode=8&opType=5&lblId=2&lblDataI=01%2F11%2F15+00%3A00&lblDataF=01%2F12%2F15+00%3A00");
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  res = curl_easy_perform(curl_handle);
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    return 0;
  }

  curl_handle = curl_easy_init();
  if(curl_handle) {

    char *url = "http://catador.herokuapp.com/records";


    json = json_object_new_object();
    json_object_object_add(json, "name", json_object_new_string("pacomunicipal.txt"));
    json_object_object_add(json, "content", json_object_new_string(chunk.memory));

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json_object_to_json_string(json));
    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "POST");
    res = curl_easy_perform(curl_handle);


    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      return 0;
    }
  }

  curl_easy_cleanup(curl_handle);
  free(chunk.memory);
  curl_global_cleanup();
  return 0;
}

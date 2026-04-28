#pragma once

#include "axmol.h"
#include "network/HttpClient.h"

#include <string>
#include <map>

using namespace std;
using namespace ax;
using namespace ax::network;

class HttpRequestHandler
{

public:
    static void sendGetRequest(std::string path, function<void(HttpClient* client, HttpResponse* response)> callback);
    static void sendPostRequest(std::string path, function<void(HttpClient* client, HttpResponse* response)> callback);
	
	static vector<int> convertBufferToVectorOfInt(yasio::sbyte_buffer* buffer);
	static string convertBufferToString(yasio::sbyte_buffer* buffer);
	static int convertBufferToInt(yasio::sbyte_buffer* buffer);

    static void setUrl(std::string url) { _url = url; }

private:
    static std::string _url;
};

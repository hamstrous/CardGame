#include "HttpRequestHandler.h"

void HttpRequestHandler::sendGetRequest(std::string url,
                                               function<void(HttpClient* client, HttpResponse* response)> callback)
{
    auto request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    request->setResponseCallback(callback);
    HttpClient::getInstance()->send(request);
    request->release();  // send() retains internally
}

void HttpRequestHandler::sendPostRequest(std::string url,
                                        function<void(HttpClient* client, HttpResponse* response)> callback)
{
    auto request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(url.c_str());
    request->setResponseCallback(callback);
    HttpClient::getInstance()->send(request);
    request->release();  // send() retains internally
}

vector<int> HttpRequestHandler::convertBufferToVectorOfInt(yasio::sbyte_buffer* buffer)
{
    string str = convertBufferToString(buffer);
    // List format: [0,1,2,...,n]
    vector<int> result;
    size_t start = 1;  // Skip the opening bracket
    size_t end = str.find(',', start);
    while (end != string::npos)
    {
        result.push_back(stoi(str.substr(start, end - start)));
        start = end + 1;
        end = str.find(',', start);
    }
    // Add the last element
    result.push_back(stoi(str.substr(start, str.length() - start - 1)));
    return result;
}

string HttpRequestHandler::convertBufferToString(yasio::sbyte_buffer* buffer)
{
    string str(buffer->data(), buffer->size());
    return str;
}

int HttpRequestHandler::convertBufferToInt(yasio::sbyte_buffer* buffer)
{
    string str = convertBufferToString(buffer);
    return stoi(str);
}


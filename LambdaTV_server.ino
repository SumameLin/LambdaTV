#include "LambdaTV.h"
/*
太极创客教程
*/
/*
函 数 名:void handleUserRequet()
功能说明:处理用户浏览器的HTTP访问
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
void handleUserRequet()
{
    // 获取用户请求网址信息
    String webAddress = esp8266_server.uri();
    // 通过handleFileRead函数处处理用户访问
    bool fileReadOK = handleFileRead(webAddress);
    // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
    if (!fileReadOK)
    {
        esp8266_server.send(404, "text/plain", "404 Not Found");
    }
}
/*
函 数 名:bool handleFileRead(String path) 
功能说明:处理浏览器HTTP访问
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
bool handleFileRead(String path)
{
    if (path.endsWith("/"))
    {                         // 如果访问地址以"/"为结尾
        path = "/index.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
    }
    String contentType = getContentType(path); // 获取文件类型
    if (SPIFFS.exists(path))
    {                                                 // 如果访问的文件可以在SPIFFS中找到
        File file = SPIFFS.open(path, "r");           // 则尝试打开该文件
        esp8266_server.streamFile(file, contentType); // 并且将该文件返回给浏览器
        file.close();                                 // 并且关闭文件
        return true;                                  // 返回true
    }
    return false; // 如果文件未找到，则返回false
}
/*
函 数 名:String getContentType(String filename)
功能说明:获取文件类型
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
String getContentType(String filename)
{
    if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}
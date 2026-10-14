#include "server.h"
#include "api.h"
#include "json.h"

static void getMimeType(const char *path, char *mimeBuf, size_t bufSize) {
    if (strstr(path, ".html")) strncpy(mimeBuf, "text/html", bufSize);
    else if (strstr(path, ".css")) strncpy(mimeBuf, "text/css", bufSize);
    else if (strstr(path, ".js")) strncpy(mimeBuf, "application/javascript", bufSize);
    else if (strstr(path, ".json")) strncpy(mimeBuf, "application/json", bufSize);
    else if (strstr(path, ".png")) strncpy(mimeBuf, "image/png", bufSize);
    else strncpy(mimeBuf, "text/plain", bufSize);
}

void startHTTPServer(int port) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("[CONFIGCORE SERVER ERROR] WSAStartup failed!\n");
        fflush(stdout);
        return;
    }
#endif

    SOCKET serverSock = INVALID_SOCKET;
    int actualPort = port;

    for (int p = port; p < port + 10; p++) {
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock == INVALID_SOCKET) {
            printf("[CONFIGCORE SERVER ERROR] Failed to create socket!\n");
            fflush(stdout);
            return;
        }

        int opt = 1;
        setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons((unsigned short)p);

        if (bind(serverSock, (struct sockaddr*)&address, sizeof(address)) != SOCKET_ERROR) {
            actualPort = p;
            break;
        }

        closesocket(serverSock);
        serverSock = INVALID_SOCKET;
    }

    if (serverSock == INVALID_SOCKET) {
        printf("[CONFIGCORE SERVER ERROR] Could not bind to ports %d..%d!\n", port, port + 9);
        fflush(stdout);
        return;
    }

    if (listen(serverSock, 10) == SOCKET_ERROR) {
        printf("[CONFIGCORE SERVER ERROR] Listen failed!\n");
        fflush(stdout);
        closesocket(serverSock);
        return;
    }

    printf("\n=======================================================\n");
    printf(" CONFIGCORE C HTTP Server active on http://localhost:%d\n", actualPort);
    printf(" Data Structures & CPQ Engine running...\n");
    printf("=======================================================\n\n");
    fflush(stdout);

    while (1) {
        struct sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSock == INVALID_SOCKET) continue;

        char buffer[4096];
        int bytesRead = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            closesocket(clientSock);
            continue;
        }
        buffer[bytesRead] = '\0';

        char method[16], urlPath[256], protocol[16];
        sscanf(buffer, "%s %s %s", method, urlPath, protocol);

        char *bodyPtr = strstr(buffer, "\r\n\r\n");
        if (bodyPtr) bodyPtr += 4;
        else bodyPtr = "";

        if (strncmp(urlPath, "/api", 4) == 0) {
            // API Route
            char *apiResponse = handleAPIRequest(method, urlPath, bodyPtr);
            char header[512];
            int bodyLen = (int)strlen(apiResponse);
            snprintf(header, sizeof(header),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: application/json\r\n"
                     "Access-Control-Allow-Origin: *\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n\r\n", bodyLen);
            send(clientSock, header, (int)strlen(header), 0);
            send(clientSock, apiResponse, bodyLen, 0);
        } else {
            // Static File Route
            char filePath[512];
            if (strcmp(urlPath, "/") == 0) {
                strcpy(filePath, "frontend/index.html");
            } else {
                snprintf(filePath, sizeof(filePath), "frontend%s", urlPath);
            }

            char *fileContent = readEntireFile(filePath);
            if (fileContent) {
                char mimeType[64];
                getMimeType(filePath, mimeType, sizeof(mimeType));
                int fileLen = (int)strlen(fileContent);

                char header[512];
                snprintf(header, sizeof(header),
                         "HTTP/1.1 200 OK\r\n"
                         "Content-Type: %s\r\n"
                         "Content-Length: %d\r\n"
                         "Connection: close\r\n\r\n", mimeType, fileLen);
                send(clientSock, header, (int)strlen(header), 0);
                send(clientSock, fileContent, fileLen, 0);
                free(fileContent);
            } else {
                const char *notFound = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
                send(clientSock, notFound, (int)strlen(notFound), 0);
            }
        }

        closesocket(clientSock);
    }

#ifdef _WIN32
    WSACleanup();
#endif
}

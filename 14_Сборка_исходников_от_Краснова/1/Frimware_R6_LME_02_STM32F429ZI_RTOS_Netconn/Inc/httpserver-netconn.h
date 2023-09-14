#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

void http_server_netconn_init(void);
void DynWebPage(struct netconn *conn, char *buf);
uint8_t checkAuthorization(struct netconn *conn, char *buf);
void sendError(struct netconn *conn, char *buf);
void sendStatusXML(struct netconn *conn, char *buf);
void sendNetsettingsXML(struct netconn *conn, char *buf);
void sendCredentialsXML(struct netconn *conn, char *buf);
void sendLogFile(struct netconn *conn, char *buf);
void actionSave(struct netconn *conn, char *buf);
void actionStartCal(struct netconn *conn, char *buf);
void actionStartMeas(struct netconn *conn, char *buf);
void actionSaveNetsettings(struct netconn *conn, char *buf);
void actionSaveCredentials(struct netconn *conn, char *buf);
void actionCheckCredentials(struct netconn *conn, char *buf);
void actionResetToDefaults(struct netconn *conn, char *buf);

#endif /* __HTTPSERVER_NETCONN_H__ */

/*
 *  3wdaytim      Simple daytime client for Win32s
 *  Copyright (c) 2019-2010 Toyoyo Toyo <toyoyo@wirefull.org>
 *
 *  This program is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

#include <windows.h>
#include <stdio.h>
#include <winsock.h>

int getmonth(char * month_str) {
  char * months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
  int ct;
  int mlen = sizeof(months)/sizeof(months[0]);
  for(ct = 0; ct < mlen; ++ct)
  {
    if(!strcmp(months[ct], month_str))
    {
      return ct+1;
    }
  }
  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {

  SYSTEMTIME st;
  struct hostent *ptrh;
  struct protoent *ptrp;
  struct sockaddr_in sad;
  struct in_addr addr;
  int sd;
  int cfg_port;
  int cfg_display;
  int cfg_set;
  int nbytes;
  int rbytes;
  int err;
  char recv_buff[64];
  char recv_total[512];
  char msgbox_str[1024];
  char cfg_server[256];
  char month_str[4];

  /* Some configuration */
  GetPrivateProfileString("Daytime", "server", "localhost", cfg_server, sizeof(cfg_server)/sizeof(cfg_server[0]), ".\\w3daytim.ini");
  cfg_port = GetPrivateProfileInt("Daytime", "port", 13, ".\\w3daytim.ini");
  cfg_display = GetPrivateProfileInt("Daytime", "display", 1, ".\\w3daytim.ini");
  cfg_set = GetPrivateProfileInt("Daytime", "set", 1, ".\\w3daytim.ini");

  /* winsock startup
     win32s supports only v1.1 (not v1.0, not v2.x) */
  WSADATA wsaData;
  err = WSAStartup(0x0101, &wsaData);
  sprintf(msgbox_str, "%d", err);
  if (err != 0) {
    MessageBox(NULL, msgbox_str, "WSAStartup Error", MB_OK);
  }

  memset((char *)&sad,0,sizeof(sad));
  sad.sin_family = AF_INET;
  sad.sin_port = htons((u_short)cfg_port);

  /* I want to support using either a FQDN a (dot-decimal) IP Address)
     gethostbyname() only support FQDN */
  addr.s_addr = inet_addr(cfg_server);
  if (addr.s_addr != INADDR_NONE)
    ptrh = gethostbyaddr((char *) &addr, 4, AF_INET);
  else
    ptrh = gethostbyname(cfg_server);

  if ( ((char *)ptrh) == NULL ) {
    MessageBox(NULL, "invalid host", "Error", MB_OK);
    exit(1);
  }

  memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  err = connect(sd, (struct sockaddr *)&sad, sizeof(sad));
  sprintf(msgbox_str, "%d", WSAGetLastError());
  if (err < 0) {
    MessageBox(NULL, msgbox_str, "connect() error", MB_OK);
    exit(1);
  }

  nbytes = 0;
  rbytes = recv(sd, recv_buff, sizeof(recv_buff)-1, 0);
  while(rbytes > 0 && nbytes+rbytes < sizeof(recv_total) ) {
    memcpy(recv_total+nbytes, recv_buff, rbytes);
    nbytes = nbytes+rbytes;
    rbytes = recv(sd, recv_buff, sizeof(recv_buff)-1, 0);
  }
  recv_total[nbytes] = 0;

  memcpy(month_str, recv_total+4, 3);
  month_str[3] = 0;

  st.wMonth = getmonth(month_str);
  st.wYear = atoi(recv_total+20);
  st.wDay = atoi(recv_total+8);
  st.wHour = atoi(recv_total+11);
  st.wMinute = atoi(recv_total+14);
  st.wSecond = atoi(recv_total+17);
  st.wMilliseconds = 0;

  if(cfg_display == 1) {
    sprintf(msgbox_str, "%s sent:\n%s\nConverted to:\n%d-%d-%d %d:%d:%d", cfg_server, recv_total, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    MessageBox(NULL, msgbox_str, "Daytime", MB_OK);
  }

  if(cfg_set == 1) {
    err=SetSystemTime(&st);
    if (!err) MessageBox(NULL, "Invalid date, SetSystemDate() failed", "Daytime", MB_OK);
  }
  closesocket(sd);

  return 0;
}

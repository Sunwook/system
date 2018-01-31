
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static int isHandShakeDone=0;

/*
typedef struct {
	char origin[255];
	char host[255];
	unsigned long key1;
	unsigned long key2;
	unsigned long key3;

} HandshakeParams;
*/

gboolean
read_socket (GIOChannel *in, GIOCondition condition, gpointer data)
{
	gint got=0, ret=0;
	char message[256]={'\0'};

	char *handShakeReply = NULL, *reply = NULL; 
	char *p=NULL;
	HandshakeParams hsParams;
	unsigned long key1=0, key2=0;
	unsigned char resultKey[16]={'\0'};

	gint local_client_socket = g_io_channel_unix_get_fd(in);

	if(condition & G_IO_HUP)
	{
		printf("\nUnexpected Broken pipe error on client_fd\n");
		close(local_client_socket);
		return FALSE;
	}


	memset(message,0,256);
	memset(&hsParams, 0, sizeof(HandshakeParams));

	if(isHandShakeDone==0)
	{
		printf("\nHandshaking..\n");

		got = recv (local_client_socket, message, 256, 0);

		if (got < 0)
		{
			printf("Failed to read from socket");
			/*TRUE becasue we still want GmainLoop to monitor this eventsource, the socket*/
			return TRUE;
		}

		printf("GOT MESSAGE:\n%s\n", message);

		collect_handshake_params(message, &hsParams);

		handShakeReply = (char*)malloc(1024); 
		memset(handShakeReply, 0, 1024);
		p = handShakeReply;

		strcpy(p,   "HTTP/1.1 101 WebSocket Protocol Handshake\x0d\x0a");
		p += strlen("HTTP/1.1 101 WebSocket Protocol Handshake\x0d\x0a");  
		strcpy(p, "Upgrade: WebSocket\x0d\x0a"); 
		p += strlen("Upgrade: WebSocket\x0d\x0a");
		strcpy(p,   "Connection: Upgrade\x0d\x0a");
		p += strlen("Connection: Upgrade\x0d\x0a");
		strcpy(p, "Sec-WebSocket-Origin: ");
		p += strlen("Sec-WebSocket-Origin: ");
		strcpy(p, hsParams.origin);
		p += strlen(hsParams.origin);
		strcpy(p,   "\x0d\x0aSec-WebSocket-Location: ws://");
		p += strlen("\x0d\x0aSec-WebSocket-Location: ws://");
		strcpy(p, hsParams.host);
		p += strlen(hsParams.host);
		strcpy(p, "/mySession\x0d\x0a");
		p += strlen("/mySession\x0d\x0a");
		strcpy(p,   "\x0d\x0a");
		p += strlen("\x0d\x0a");



		if((ret=interpret_key(hsParams.key1,&key1)) < 0)
		{
			printf("\nError in parsing key1! Errcode=%d\n",ret);
			return FALSE;
		} 
		else
		{
			printf("\nStripped Key1 = %ld\n", key1);
		}

		if((ret=interpret_key(hsParams.key2,&key2)) < 0)
		{
			printf("\nError in parsing key2! Errcode=%d\n",ret);
			return FALSE;
		} 
		else
		{
			printf("\nStripped Key2 = %ld\n", key2);
		}

		//Arrange in Network Byte Order!
		resultKey[0] = key1 >> 24; 
		resultKey[1] = key1 >> 16;
		resultKey[2] = key1 >> 8;
		resultKey[3] = key1;
		resultKey[4] = key2 >> 24;
		resultKey[5] = key2 >> 16;
		resultKey[6] = key2 >> 8;
		resultKey[7] = key2;

		memcpy(&resultKey[8], hsParams.key3, 8);

		copyMD5Hash(resultKey, (unsigned char *)p);
		p += 16;

		printf("\nHandshake Reply:\n%s\n", handShakeReply);

		ret = send(local_client_socket, handShakeReply, p-handShakeReply, 0);
		if(ret < 0)
		{
			perror("\nError in sending handshake reply:");
		}
		else
		{
			isHandShakeDone=1;
		}

		free(handShakeReply);
		p=NULL;
		free_handshake_params(&hsParams);

	}
	else
	{
		printf("\nServing Client..\n");

		getClientRequest(local_client_socket, message);

		if(message[0] == '\0')
		{
			/*Close this connection for this client instance and wait for connection from the next instance of client*/

			close(local_client_socket);

#if 0   
			GIOStatus status;          
			GError *error=NULL;

			status = g_io_channel_shutdown(in,TRUE,&error);
			if(status==G_IO_STATUS_NORMAL)
			{
				printf("\nClient IO channel shutdown was normal\n");
			}
			else
			{
				printf("\nIO channel shutdown status: %d\n", status);
			}


			/*Unref twice since g_io_add_watch() increases the reference count too*/
			g_io_channel_unref(in);
			g_io_channel_unref(in);
#endif     

			isHandShakeDone=0;
			return TRUE;
		}

		reply = (char*)malloc(256); 
		memset(reply, 0, 1024);
		p = reply;

		*p = 0;
		p++;

		strcpy(p,"This is the server's reply!");
		p += strlen("This is the server's reply!");

		*p = 255;
		p++;

		ret = send(local_client_socket, reply, p-reply, 0);
		if(ret < 0)
		{
			perror("\nError in sending response:");
		}

	}

	return TRUE;

}



	gboolean
handle_socket(GIOChannel *in, GIOCondition condition, gpointer data)
{
	GIOChannel *client_channel;
	gint client_socket;

	gint socket_fd = g_io_channel_unix_get_fd(in);

	if(condition & G_IO_HUP)
	{
		printf("\nUnexpected Broken pipe error on socket_fd\n");
		close(socket_fd);
		return FALSE;
	}

	if(isHandShakeDone==0)
	{

		client_socket = accept (socket_fd, NULL, NULL);

		if (client_socket < 0)
		{
			g_print("ERROR CLIENT_SOCKET VALUE!!!!!!!!!!!!!!!!!!!!");
			return FALSE;
		}


		client_channel = NULL;


		/*Program crashes on this call*/
		client_channel = g_io_channel_unix_new (client_socket);
		g_io_channel_set_encoding (client_channel, NULL, NULL);
		g_io_channel_set_buffered (client_channel, FALSE);


		g_io_add_watch (client_channel, G_IO_IN | G_IO_HUP, (GIOFunc) read_socket, NULL);
	}

	return TRUE;
}

int main(int argc, char **argv)
{

	GIOChannel *channel_socket;
	gint socket_fd;
	static GMainLoop *loop = NULL;
	struct sockaddr_in serv_addr;

	g_type_init();

	isHandShakeDone=0;

	socket_fd = socket (AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) 
	{
		g_print("Error creating socket\n");
		exit (1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(49059);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind (socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		g_print("Error binding socket");
		exit(2);
	}

	listen (socket_fd, 5);

	channel_socket = g_io_channel_unix_new (socket_fd);
	g_io_channel_set_encoding (channel_socket, NULL, NULL);
	g_io_channel_set_buffered (channel_socket, FALSE);

	g_io_add_watch (channel_socket, G_IO_IN | G_IO_HUP, (GIOFunc) handle_socket, NULL);

	g_print("GOING INTO MAINLOOP\n");

	loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (loop);
}

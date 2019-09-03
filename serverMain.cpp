#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "4040"
#define MAP_SIZE 12

typedef struct ThreadData {
	SOCKET ClientSocket1;
	SOCKET ClientSocket2;
	int i;
}ThreadData;

typedef struct mesageData {
	char message[DEFAULT_BUFLEN];
	int x, y;
} messageData;




int Damaged(char map[][MAP_SIZE], int x, int y, int counter) {
	if (counter == 4)
		return 0;
	if (map[y][x + 1] == '+')
		return 1;
	if (map[y][x - 1] == '+')
		return 1;
	if (map[y + 1][x] == '+')
		return 1;
	if (map[y - 1][x] == '+')
		return 1;
	int result1 = 0;
	int result2 = 0;
	int result3 = 0;
	int result4 = 0;
	if (map[y][x + 1] == 'x')
		result1 = Damaged(map, x + 1, y, counter + 1);
	if (map[y][x - 1] == 'x')
		result1 = Damaged(map, x - 1, y, counter + 1);
	if (map[y + 1][x] == 'x')
		result1 = Damaged(map, x, y + 1, counter + 1);
	if (map[y - 1][x] == 'x')
		result1 = Damaged(map, x, y - 1, counter + 1);
	return result1 || result2 || result3 || result4;
}

int SendMess(SOCKET ClientSocket, messageData *data) {
	int iResult = send(ClientSocket, (char*)data, sizeof(messageData), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		return 1;
	}
	return 0;
}

int GetMess(SOCKET ClientSocket, messageData *data) {
	int iResult = recv(ClientSocket, (char*)data, sizeof(messageData), 0);
	if (iResult <= 0) {
		printf("Connection closed\n");
		return 1;
	}
	return 0;
}



DWORD WINAPI GameSeaBattle(LPVOID lpParam) {
	char client1[MAP_SIZE][MAP_SIZE];
	char client2[MAP_SIZE][MAP_SIZE];
	messageData msData;
	messageData errorData;
	int shipcounter1 = 10;
	int shipcounter2 = 10;
	ThreadData* data = (ThreadData*)lpParam;
	SOCKET ClientSocket1 = data->ClientSocket1;
	SOCKET ClientSocket2 = data->ClientSocket2;
	int numberOfGame = data->i;
	free(data);

	strcpy(msData.message, "Ok");
	strcpy(errorData.message, "Error");
	if (SendMess(ClientSocket1, &msData)) {
		SendMess(ClientSocket2, &errorData);
		return 1;
	}
	if (SendMess(ClientSocket2, &msData)) {
		SendMess(ClientSocket1, &errorData);
		return 1;
	}
	int iResult = recv(ClientSocket1, (char*)&client1, sizeof(client1), 0);
	if (iResult <= 0) {
		printf("Connection closed\n");
		SendMess(ClientSocket2, &errorData);
		return 1;
	}
	iResult = recv(ClientSocket2, (char*)&client2, sizeof(client1), 0);
	if (iResult <= 0) {
		printf("Connection closed\n");
		SendMess(ClientSocket1, &errorData);
		return 1;
	}
	
	int flag = 1;
	while (flag) {
		if (flag == 1) {
			strcpy(msData.message, "Turn");
			if (SendMess(ClientSocket1, &msData)) {
				SendMess(ClientSocket2, &errorData);
				return 1;
			}
			strcpy(msData.message, "Wait");
			if (SendMess(ClientSocket2, &msData)) {
				SendMess(ClientSocket1, &errorData);
				return 1;
			}
			if (GetMess(ClientSocket1, &msData)) {
				SendMess(ClientSocket2, &errorData);
				return 1;
			}
			if (client2[msData.y][msData.x] == '+') {
				client2[msData.y][msData.x] = 'x';
				if (Damaged(client2, msData.x, msData.y, 1)) {
					strcpy(msData.message, "hit");
					if (SendMess(ClientSocket1, &msData)) {
						SendMess(ClientSocket2, &errorData);
						return 1;
					}
					strcpy(msData.message, "yhit");
					if (SendMess(ClientSocket2, &msData)) {
						SendMess(ClientSocket1, &errorData);
						return 1;
					}
				}
				else {
					--shipcounter2;
					if (shipcounter2) {
						strcpy(msData.message, "dest");
						if (SendMess(ClientSocket1, &msData)) {
							SendMess(ClientSocket2, &errorData);
							return 1;
						}
						strcpy(msData.message, "ydest");
						if (SendMess(ClientSocket2, &msData)) {
							SendMess(ClientSocket1, &errorData);
							return 1;
						}
					}
					else {
						strcpy(msData.message, "win");
						if (SendMess(ClientSocket1, &msData)) {
							SendMess(ClientSocket2, &errorData);
							return 1;
						}
						strcpy(msData.message, "lose");
						if (SendMess(ClientSocket2, &msData)) {
							SendMess(ClientSocket1, &errorData);
							return 1;
						}
						flag = 0;
					}
				}
			}
			else {
				client2[msData.y][msData.x] = '-';
				strcpy(msData.message, "miss");
				if (SendMess(ClientSocket1, &msData)) {
					SendMess(ClientSocket2, &errorData);
					return 1;
				}
				strcpy(msData.message, "ymiss");
				if (SendMess(ClientSocket2, &msData)) {
					SendMess(ClientSocket1, &errorData);
					return 1;
				}
				flag = 2;
			}
		}
		else if (flag == 2) {
			strcpy(msData.message, "Turn");
			if (SendMess(ClientSocket2, &msData)) {
				SendMess(ClientSocket1, &errorData);
				return 1;
			}
			strcpy(msData.message, "Wait");
			if (SendMess(ClientSocket1, &msData)) {
				SendMess(ClientSocket2, &errorData);
				return 1;
			}
			if (GetMess(ClientSocket2, &msData)) {
				SendMess(ClientSocket1, &errorData);
				return 1;
			}
			if (client1[msData.y][msData.x] == '+') {
				client1[msData.y][msData.x] = '-';
				if (Damaged(client1, msData.x, msData.y, 1)) {
					strcpy(msData.message, "hit");
					if (SendMess(ClientSocket2, &msData)) {
						SendMess(ClientSocket1, &errorData);
						return 1;
					}
					strcpy(msData.message, "yhit");
					if (SendMess(ClientSocket1, &msData)) {
						SendMess(ClientSocket2, &errorData);
						return 1;
					}
				}
				else {
					--shipcounter1;
					if (shipcounter1) {
						strcpy(msData.message, "dest");
						if (SendMess(ClientSocket2, &msData)) {
							SendMess(ClientSocket1, &errorData);
							return 1;
						}
						strcpy(msData.message, "ydest");
						if (SendMess(ClientSocket1, &msData)) {
							SendMess(ClientSocket2, &errorData);
							return 1;
						}
					}
					else {
						strcpy(msData.message, "win");
						if (SendMess(ClientSocket2, &msData)) {
							SendMess(ClientSocket1, &errorData);
							return 1;
						}
						strcpy(msData.message, "lose");
						if (SendMess(ClientSocket1, &msData)) {
							SendMess(ClientSocket2, &errorData);
							return 1;
						}
						flag = 0;
					}
				}
			}
			else {
				client1[msData.y][msData.x] = '-';
				strcpy(msData.message, "miss");
				if (SendMess(ClientSocket2, &msData)) {
					SendMess(ClientSocket1, &errorData);
					return 1;
				}
				strcpy(msData.message, "ymiss");
				if (SendMess(ClientSocket1, &msData)) {
					SendMess(ClientSocket2, &errorData);
					return 1;
				}
				flag = 1;
			}
		}
	}
	printf("Game [%d]  ended.\n", numberOfGame + 1);
	closesocket(ClientSocket1);
	closesocket(ClientSocket2);
	return 0;
}




int  main(int argc, char **argv) {
	WSADATA wsaData;
	int iResult;
	char port[32];
	if (argc < 2) 
		memcpy(port, DEFAULT_PORT, sizeof(DEFAULT_PORT));
	else
		memcpy(port, argv[2], sizeof(DEFAULT_PORT));

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);

	DWORD   dwThreadIdArray[MAXIMUM_WAIT_OBJECTS];
	HANDLE  hThreadArray[MAXIMUM_WAIT_OBJECTS];
	int threadCounter = 0;
	while (1) {
		if (threadCounter + 1 == MAXIMUM_WAIT_OBJECTS) {
			WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, hThreadArray, FALSE, INFINITE);
		}
		ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData));
		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		data->ClientSocket1 = accept(ListenSocket, NULL, NULL);
		if (data->ClientSocket1 == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		printf("Client [%d] connect to server \n", threadCounter * 2 + 1);
		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		data->ClientSocket2 = accept(ListenSocket, NULL, NULL);
		if (data->ClientSocket2 == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		printf("Client [%d] connect to server \n", threadCounter * 2 + 2);
		data->i = threadCounter;
		hThreadArray[threadCounter] = CreateThread(NULL, 0,
			GameSeaBattle,
			(LPVOID)data,
			0,
			&dwThreadIdArray[threadCounter]);
		++threadCounter;
		printf("Game [%d] created! \n", threadCounter);
	}
	closesocket(ListenSocket);
	WSACleanup();
	
	return 0;
}
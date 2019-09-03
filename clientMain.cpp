#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 

#define DEFAULT_BUFLEN 512
#define MAP_SIZE 12
#define DEFAULT_PORT "4040"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

typedef struct mesageData {
	char message[DEFAULT_BUFLEN];
	int x, y;
} messageData;

char Map[MAP_SIZE][MAP_SIZE];
char enemyMap[MAP_SIZE][MAP_SIZE];

void printMap(char map[][MAP_SIZE]) {
	printf("_________________________\n");
	printf("|  | 1 2 3 4 5 6 7 8 9 10\n");
	for (int i = 1; i < MAP_SIZE-1; ++i) {
		printf("|%2d|", i);
		for (int j = 1; j < MAP_SIZE-1; ++j) 
			printf(" %c", map[i][j]);
		printf("\n");
	}
	printf("|__|_____________________\n");
}

int checkWay(int startX, int endX, int startY, int endY, char symbol) {
	if (startX > 10 || startX < 1 || startY > 10 || startY < 1
		|| endX > 10 || endX < 1 || endY > 10 || endY < 1)
		return 0;
	if (startY != endY) {
		for (int i = min(startY, endY); i <= max(startY, endY); ++i)
			if (Map[i][startX] == symbol
				|| Map[i + 1][startX] == symbol
				|| Map[i - 1][startX] == symbol
				|| Map[i][startX + 1] == symbol
				|| Map[i][startX - 1] == symbol) {
				return 1;
			}
	}
	else if (startX != endX) {
		for (int i = min(startX, endX); i <= max(startX, endX); ++i)
			if (Map[startY][i] == symbol
				|| Map[startY][i + 1] == symbol
				|| Map[startY][i - 1] == symbol
				|| Map[startY + 1][i] == symbol
				|| Map[startY - 1][i] == symbol) {
				return 1;
			}
	}
	else {
		if (Map[startY][startX] == symbol
			|| Map[startY + 1][startX] == symbol
			|| Map[startY - 1][startX] == symbol
			|| Map[startY][startX + 1] == symbol
			|| Map[startY][startX - 1] == symbol) {
			return 1;
		}
	}
	return 0;
}

void initWay(int startX, int endX, int startY, int endY, char symbol) {
	if (startY != endY) {
		for (int i = min(startY, endY); i <= max(startY, endY); ++i)
			Map[i][startX] = symbol;
	}
	else if (startX != endX) {
		for (int i = min(startX, endX); i <= max(startX, endX); ++i)
			Map[startY][i] = symbol;
	}
	else
		Map[startY][startX] = symbol;
}

void readFile() {
	FILE *f = fopen("C:\\Users\\RED\\Documents\\Labs\\OS\\OSKP\\Debug\\map.txt", "r");
	
	int startX, endX, startY, endY;
	for (int i = 0; i < 10; ++i) {
		fscanf(f, "%d %d %d %d", &startX, &endX, &startY, &endY);
		initWay(startX, startY, endX, endY, '+');
	}
	fclose(f);
}


void initMap() {
	int startX, endX, startY, endY;
	printMap(Map);
	printf("Enter the coordinates of  battleship.\nStarting point  [x] [y]:");
	scanf("%d %d", &startX, &startY);
	printf("End point   [x] [y]:");
	scanf("%d %d", &endX, &endY);
	while (startX > 10 || startX < 1 || startY > 10 || startY < 1 
			|| endX > 10 || endX < 1 || endY > 10 || endY < 1
			|| !((startX == endX && abs(startY - endY) == 3) ^ (startY == endY && abs(startX - endX) == 3))
			|| checkWay(startX, endX, startY, endY, '+')) {

		printf("Wrong coordinates!\nStarting point  [x] [y]:" );
		scanf("%d %d", &startX, &startY);
		printf("End point   [x] [y]:");
		scanf("%d %d", &endX, &endY);
	}
	initWay(startX, endX, startY, endY, '+');
	system("cls");
	printMap(Map);
	for (int i = 0; i < 2; ++i) {
		printf("Enter the coordinates of  cruiser[%d].\nStarting point  [x] [y]:", i+1);
		scanf("%d %d", &startX, &startY);
		printf("End point   [x] [y]:");
		scanf("%d %d", &endX, &endY);
		while (startX > 10 || startX < 1 || startY > 10 || startY < 1
			|| endX > 10 || endX < 1 || endY > 10 || endY < 1
			|| !((startX == endX && abs(startY - endY) == 2) ^ (startY == endY && abs(startX - endX) == 2))
			|| checkWay(startX, endX, startY, endY, '+')) {

			printf("Wrong coordinates!\nStarting point  [x] [y]:");
			scanf("%d %d", &startX, &startY);
			printf("End point   [x] [y]:");
			scanf("%d %d", &endX, &endY);
		}
		initWay(startX, endX, startY, endY, '+');
		system("cls");
		printMap(Map);
	}
	for (int i = 0; i < 3; ++i) {
		printf("Enter the coordinates of  destroyer[%d].\nStarting point  [x] [y]:", i + 1);
		scanf("%d %d", &startX, &startY);
		printf("End point   [x] [y]:");
		scanf("%d %d", &endX, &endY);
		while (startX > 10 || startX < 1 || startY > 10 || startY < 1
			|| endX > 10 || endX < 1 || endY > 10 || endY < 1
			|| !((startX == endX && abs(startY - endY) == 1) ^ (startY == endY && abs(startX - endX) == 1))
			|| checkWay(startX, endX, startY, endY, '+')) {

			printf("Wrong coordinates!\nStarting point  [x] [y]:");
			scanf("%d %d", &startX, &startY);
			printf("End point   [x] [y]:");
			scanf("%d %d", &endX, &endY);
		}
		initWay(startX, endX, startY, endY, '+');
		system("cls");
		printMap(Map);
	}
	for (int i = 0; i < 4; ++i) {
		printf("Enter the coordinates of  cutter[%d].\nPoint  [x] [y]:", i + 1);
		scanf("%d %d", &startX, &startY);
		while (startX > 10 || startX < 1 || startY > 10 || startY < 1
			|| checkWay(startX, startX, startY, startY, '+')) {

			printf("Wrong coordinates!\nPoint  [x] [y]:");
			scanf("%d %d", &startX, &startY);
		}
		initWay(startX, startX, startY, startY, '+');
		system("cls");
		printMap(Map);
	}
}



int SendMess(SOCKET ClientSocket, messageData *data) {
	int iResult = send(ClientSocket, (char*)data, sizeof(messageData), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int GetMess(SOCKET ClientSocket, messageData *data) {
	int iResult = recv(ClientSocket, (char*)data, sizeof(messageData), 0);
	if (iResult <= 0) {
		printf("Connection closed\n");
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}



int  main(int argc, char **argv) {

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	int iResult;
	char port[32];
	
	if (argc < 2) {
		printf("Enter server name!\n");
		return 1;
	}
	else if (argc == 2) 
		memcpy(port, DEFAULT_PORT, sizeof(DEFAULT_PORT));
	else 
		memcpy(port, argv[2], sizeof(DEFAULT_PORT));
	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
		
	iResult = getaddrinfo(argv[1], port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	ptr = result;
	for (;;) {
 		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	messageData data;
	printf("Waiting   response from the server... \n");
	if (GetMess(ConnectSocket, &data))
		return 1;
	if (!strcmp(data.message, "Ok"))
		printf("The game's starting! \n");
	else {
		printf("Unknown error\n");
		return 1;
	}

	for (int i = 0; i < MAP_SIZE; ++i) {
		memset(Map[i], '0', sizeof(char) * MAP_SIZE);
		memset(enemyMap[i], '0', sizeof(char) * MAP_SIZE);
	}
	//initMap();
	readFile();

	iResult = send(ConnectSocket, (char*)&Map, sizeof(Map), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	char message[128];
	int flag = 1;
	system("cls");
	printf("You map:\n");
	printMap(Map);
	printf("Enemy map:\n");
	printMap(enemyMap);
	while (flag) {
		if (GetMess(ConnectSocket, &data))
			return 1;
		
		if (!strcmp(data.message, "Turn")) {
			printf("Enter coordinates [x] [y]:");
			fflush(stdin);
			scanf("%d %d", &data.x, &data.y);
			while (data.x > 10 || data.x < 1 || data.y > 10 || data.y < 1) {
				printf("\nWrong coordinates! Try again:");
				scanf("%d %d", &data.x, &data.y);
			}
			if (SendMess(ConnectSocket, &data)) {
				return 1;
			}
			if (GetMess(ConnectSocket, &data))
				return 1;
			if (!strcmp(data.message, "hit")) {
				enemyMap[data.y][data.x] = 'x';
				printf("Hit!\n");
			}
			else if (!strcmp(data.message, "dest")) {
				enemyMap[data.y][data.x] = 'x';
				printf("Destroyed!\n");
			}
			else if (!strcmp(data.message, "miss")) {
				enemyMap[data.y][data.x] = '-';
				printf("You missed!\n");
			}
			else if (!strcmp(data.message, "win")) {
				enemyMap[data.y][data.x] = 'x';
				printf("You win!!! Game over.\n ");
				flag = 0;
			}
			else {
				printf("Unknow error in server!\n");
				flag = 0;
			}
		}
		else if (!strcmp(data.message, "yhit")) {
			Map[data.y][data.x] = 'x';
			printf("Your ship is damaged!\nCoordinates: %d %d \n", data.x, data.y);
			
		}
		else if (!strcmp(data.message, "ydest")) {
			Map[data.y][data.x] = 'x';
			printf("Your ship is destroyed!\nCoordinates: %d %d \n", data.x, data.y);
		}
		else if (!strcmp(data.message, "ymiss")) {
			Map[data.y][data.x] = '-';
			printf("Enemy missed! You turn.\nCoordinates: %d %d \n", data.x, data.y);
		}
		else if (!strcmp(data.message, "lose")) {
			Map[data.y][data.x] = 'x';
			printf("You lose! Game over.\n  ");
			flag = 0;
		}
		else if (!strcmp(data.message, "Wait")) {
			printf("Waiting  the actions of  second player...\n");
		}
		else if (!strcmp(data.message, "Error")) {
			printf("Error: Player2  disconnected...\n");
			flag = 0;
		}
		else {
			printf("Unknow error in server!\n");
			flag = 0;
		}
		printf("Press enter to continue");
		_getch();
		system("cls");
		printf("You map:\n");
		printMap(Map);
		printf("Enemy map:\n");
		printMap(enemyMap);

	}
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

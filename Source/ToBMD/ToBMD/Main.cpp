#include "Library.h"
#include "Item.h"

#define SERVER_DATA_PATH	"..\\..\\Server\\Data\\"
#define CLIENT_DATA_PATH	"..\\..\\Client\\Data\\"

int main()
{
	Item.Convert(SERVER_DATA_PATH "Item\\Item.txt", CLIENT_DATA_PATH "Local\\Item.bmd", CLIENT_DATA_PATH "Local\\Item.bmdx");

	return 0;
}
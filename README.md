# Database
A simple database system

### __We use a row to store the information: id, username, email.__
```C
typedef struct {
		uint32_t id;
		char username[COLUMN_USERNAME_SIZE];
		char email[COLUMN_EMAIL_SIZE];
}Row;
```
The insert statements are going to look like this: 
__"insert 12345 xxx xxx@gmail.com"__
| column| type
|-|-|
|id|integer|
username|varchar(32)|
|email|varchar(255)|

---
### The compact of representation of a row:
```C
uint32_t ID_SIZE;
uint32_t USERNAME_SIZE;
uint32_t EMAIL_SIZE;
uint32_t ID_OFFSET = 0;
uint32_t USERNAME_OFFSET;
uint32_t EMAIL_OFFSET;
uint32_t ROW_SIZE;
```
__This means the layout of a serialized row will look like this:__
|column|size(bytes)|offset|
|-|-|-|
|id|4|0|
|username|32|4|
|email|255|36|
|total|291|

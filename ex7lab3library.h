#ifndef EX7LAB3LIBRARY_H
#define EX7LAB3LIBRARY_H
#include "stringLibrary.h"

typedef enum gender { GENDER_MAN, GENDER_WOMAN } gender;

typedef struct Liver {
	String surname;
	String name;
	String patronymic;
	gender genderLiver;
	int dayBirthday;
	int monthBirthday;
	int yearBirthday;
	double salary;
} Liver;

typedef struct LiverNode {
	Liver currentLiver;
	int LiverID;
	struct LiverNode* next;
} LiverNode;

typedef enum answer {
	ANSWER_YES,
	ANSWER_NO,
	ANSWER_UNKNOWN,
	ANSWER_SURNAME,
	ANSWER_NAME,
	ANSWER_PATRONYMIC,
	ANSWER_BIRTHDAY,
	ANSWER_SALARY,
	ANSWER_GENDER
} answer;

typedef struct EditReminder {
	String changedString;
	gender changedGender;
	int day;
	int month;
	int year;
	double salary;
	answer changedOption;
	int id;
} EditReminder;

typedef enum statusCode { NORMAL, MEMORY_ALLOCATION_ERROR, FILE_OPEN_ERROR, INCORRECT_DATA } statusCode;

typedef enum constants {
	START_LENGTH_BUFFER = 100,
	START_LENGTH_NAME = 10,
	START_LENGTH_NUMBER = 10,
	START_AMOUNT_LIVERS = 50,
	MAX_LENGTH_OPTION = 11,
	START_LENGTH_STRING = 10,
	START_AMOUNT_OPTIONS = 10
} constants;


typedef enum options {
	OPTION_NONE,
	OPTION_MENU,
	OPTION_END,
	OPTION_FIND,
	OPTION_EDIT,
	OPTION_DELETE,
	OPTION_ADD,
	OPTION_WRITE_TO_FILE,
	OPTION_CANCEL
} options;



statusCode FillArrayLivers(Liver** liverData, int* countLivers, char* nameInputFile);
void FreeLiverData(Liver** liverData, int amountLivers);

statusCode GetSurnameFromString(const char* string, char** surname);
statusCode GetNameFromString(const char* string, char** name);
statusCode GetPatronymicFromString(const char* string, char** patronymic);
void GetBirthdayFromString(const char* string, int* day, int* month, int* year);
void GetGenderFromString(const char* string, gender* current);
statusCode GetSalaryFromString(const char* string, double* salary);

statusCode GetSurnameFromConsole(String* surname);
answer GetAnswer();
options GetCurrentOption();
void GetMenu();
statusCode GetString(char** currentString);
statusCode GettingSurname(String* surname);
statusCode GettingName(String* name);
statusCode GettingBirthday(int* day, int* month, int* year);
statusCode GettingSalary(double* salary);
statusCode GettingPatronymic(String* patronymic);
statusCode GettingGender(gender* current);

statusCode IsValidBirthday(int day, int month, int year);
statusCode IsValidName(char* name);
statusCode IsValidString(char* string);
statusCode IsValidFileName(const char* string);

int CompareLiversDecrease(const void* first, const void* second);
int CompareLiversIncrease(const void* first, const void* second);

LiverNode* CreateLiverNode(String* surname, String* name, String* patronymic, int day, int month, int year,
                           double salary, int liverID, gender liverGender);
void InsertLiverNode(LiverNode* previousNode, LiverNode* newNode);
void DeleteLiverNode(LiverNode* previousNode);
void DeleteLiverList(LiverNode** head);
void DeleteLiverByID(LiverNode* head, int id);
void PushLiverNode(LiverNode** head, LiverNode* newNode);

statusCode ConsoleReadLatin(char** currentString);
void FindParameters();
answer GetParameterFind();
void ChangeParameters();
void PrintNameFound(char* resultScan, LiverNode* head);
int PrintSurnameFound(char* resultScan, LiverNode* head);
statusCode ConsoleReadStr(char** currentString);
statusCode ConsoleReadBirthday(int * day, int* month, int * year);
statusCode ConsoleReadDouble(double* result);
statusCode ConsoleReadInteger(int* result);
void PrintPatronymicFound(char* resultScan, LiverNode* head);
void PrintBirthdayFound(int day, int month, int year, LiverNode* head);
void PrintSalaryFound(double salary, double epsilon, LiverNode* head);
void PrintGenderFound(gender current, LiverNode* head);

void FreeEditReminders(EditReminder ** reminders, int amount);
#endif
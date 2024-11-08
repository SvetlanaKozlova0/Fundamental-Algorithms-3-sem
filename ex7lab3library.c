#include "ex7lab3library.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stringLibrary.h"

statusCode FillArrayLivers(Liver** liverData, int* countLivers, char* nameInputFile) {
	FILE* inputFile = fopen(nameInputFile, "r");
	if (inputFile == NULL) {
		return FILE_OPEN_ERROR;
	}
	*liverData = (Liver*)malloc(sizeof(Liver) * START_AMOUNT_LIVERS);
	if (*liverData == NULL) {
		fclose(inputFile);
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = '0';
	int bufferSize = START_LENGTH_BUFFER;
	int bufferIndex = 0;
	char* buffer = (char*)malloc(sizeof(char) * START_LENGTH_BUFFER);
	if (buffer == NULL) {
		fclose(inputFile);
		free(*liverData);
		return MEMORY_ALLOCATION_ERROR;
	}
	int amountLivers = 0;
	int lengthLivers = START_AMOUNT_LIVERS;
	statusCode status = NORMAL;
	while ((c = getc(inputFile)) != EOF) {
		if (c == '\r' || c == '\n') {
			if (bufferIndex == 0) {
				continue;
			}
			buffer[bufferIndex] = '\0';
			bufferIndex = 0;
			if (IsValidString(buffer) == INCORRECT_DATA) {
				free(buffer);
				FreeLiverData(liverData, amountLivers);
				return INCORRECT_DATA;
			}
			char* surname = NULL;
			status = GetSurnameFromString(buffer, &surname);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(inputFile);
				FreeLiverData(liverData, amountLivers);
				return MEMORY_ALLOCATION_ERROR;
			}
			char* name = NULL;
			status = GetNameFromString(buffer, &name);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(inputFile);
				free(surname);
				FreeLiverData(liverData, amountLivers);
				return MEMORY_ALLOCATION_ERROR;
			}
			char* patronymic = NULL;
			status = GetPatronymicFromString(buffer, &patronymic);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(inputFile);
				free(surname);
				free(name);
				FreeLiverData(liverData, amountLivers);
				return MEMORY_ALLOCATION_ERROR;
			}
			int day, month, year;
			GetBirthdayFromString(buffer, &day, &month, &year);
			if (IsValidBirthday(day, month, year) == INCORRECT_DATA) {
				FreeLiverData(liverData, amountLivers);
				free(buffer);
				fclose(inputFile);
				free(surname);
				free(name);
				free(patronymic);
				return INCORRECT_DATA;
			}
			gender currentGender;
			GetGenderFromString(buffer, &currentGender);
			double salary;
			status = GetSalaryFromString(buffer, &salary);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(inputFile);
				free(name);
				free(surname);
				free(patronymic);
				FreeLiverData(liverData, amountLivers);
				return MEMORY_ALLOCATION_ERROR;
			}
			String stringSurname = CreateString(surname);
			(*liverData)[amountLivers].surname.symbols = NULL;
			CopyStringsOld(&(*liverData)[amountLivers].surname, &stringSurname);
			DestroyString(&stringSurname);
			free(surname);

			String stringName = CreateString(name);
			(*liverData)[amountLivers].name.symbols = NULL;
			CopyStringsOld(&(*liverData)[amountLivers].name, &stringName);
			DestroyString(&stringName);
			free(name);

			String stringPatronymic = CreateString(patronymic);
			(*liverData)[amountLivers].patronymic.symbols = NULL;
			CopyStringsOld(&(*liverData)[amountLivers].patronymic, &stringPatronymic);
			DestroyString(&stringPatronymic);
			free(patronymic);

			(*liverData)[amountLivers].dayBirthday = day;
			(*liverData)[amountLivers].monthBirthday = month;
			(*liverData)[amountLivers].yearBirthday = year;
			(*liverData)[amountLivers].salary = salary;
			(*liverData)[amountLivers].genderLiver = currentGender;
			amountLivers++;
			if (amountLivers + 1 >= lengthLivers) {
				lengthLivers *= 2;
				Liver* tempLivers = (Liver*)realloc(*liverData, sizeof(Liver) * lengthLivers);
				if (tempLivers == NULL) {
					free(*liverData);
					free(buffer);
					return MEMORY_ALLOCATION_ERROR;
				}
				*liverData = tempLivers;
			}
		} else {
			if (bufferIndex >= bufferSize - 1) {
				bufferSize *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * bufferSize);
				if (tempBuffer == NULL) {
					free(buffer);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[bufferIndex++] = (char)c;
		}
	}
	if (bufferIndex > 0) {
		buffer[bufferIndex] = '\0';
		if (IsValidString(buffer) == INCORRECT_DATA) {
			free(buffer);
			FreeLiverData(liverData, amountLivers);
			return INCORRECT_DATA;
		}
		char* surname = NULL;
		status = GetSurnameFromString(buffer, &surname);
		if (status == MEMORY_ALLOCATION_ERROR) {
			free(buffer);
			fclose(inputFile);
			FreeLiverData(liverData, amountLivers);
			return MEMORY_ALLOCATION_ERROR;
		}
		char* name = NULL;
		status = GetNameFromString(buffer, &name);
		if (status == MEMORY_ALLOCATION_ERROR) {
			free(buffer);
			fclose(inputFile);
			free(surname);
			FreeLiverData(liverData, amountLivers);
			return MEMORY_ALLOCATION_ERROR;
		}
		char* patronymic = NULL;
		status = GetPatronymicFromString(buffer, &patronymic);
		if (status == MEMORY_ALLOCATION_ERROR) {
			free(buffer);
			fclose(inputFile);
			free(surname);
			free(name);
			FreeLiverData(liverData, amountLivers);
			return MEMORY_ALLOCATION_ERROR;
		}
		int day, month, year;
		GetBirthdayFromString(buffer, &day, &month, &year);
		if (IsValidBirthday(day, month, year) == INCORRECT_DATA) {
			FreeLiverData(liverData, amountLivers);
			free(buffer);
			fclose(inputFile);
			free(surname);
			free(name);
			free(patronymic);
			return INCORRECT_DATA;
		}
		gender currentGender;
		GetGenderFromString(buffer, &currentGender);
		double salary;
		status = GetSalaryFromString(buffer, &salary);
		if (status == MEMORY_ALLOCATION_ERROR) {
			free(buffer);
			fclose(inputFile);
			free(name);
			free(surname);
			free(patronymic);
			FreeLiverData(liverData, amountLivers);
			return MEMORY_ALLOCATION_ERROR;
		}
		String stringSurname = CreateString(surname);
		(*liverData)[amountLivers].surname.symbols = NULL;
		CopyStringsOld(&(*liverData)[amountLivers].surname, &stringSurname);
		DestroyString(&stringSurname);
		free(surname);

		String stringName = CreateString(name);
		(*liverData)[amountLivers].name.symbols = NULL;
		CopyStringsOld(&(*liverData)[amountLivers].name, &stringName);
		DestroyString(&stringName);
		free(name);

		String stringPatronymic = CreateString(patronymic);
		(*liverData)[amountLivers].patronymic.symbols = NULL;
		CopyStringsOld(&(*liverData)[amountLivers].patronymic, &stringPatronymic);
		DestroyString(&stringPatronymic);
		free(patronymic);

		(*liverData)[amountLivers].dayBirthday = day;
		(*liverData)[amountLivers].monthBirthday = month;
		(*liverData)[amountLivers].yearBirthday = year;
		(*liverData)[amountLivers].salary = salary;
		amountLivers++;
		if (amountLivers + 1 >= lengthLivers) {
			lengthLivers *= 2;
			Liver* tempLivers = (Liver*)realloc(*liverData, sizeof(Liver) * lengthLivers);
			if (tempLivers == NULL) {
				free(*liverData);
				free(buffer);
				return MEMORY_ALLOCATION_ERROR;
			}
			*liverData = tempLivers;
		}
	}
	free(buffer);
	fclose(inputFile);
	*countLivers = amountLivers;
	return NORMAL;
}

statusCode IsValidString(char* string) {
	int index = 0;
	int flagIsSurname = 0;
	int flagIsName = 0;
	int flagIsBirthday = 0;
	int flagIsGender = 0;
	int flagIsSalary = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) == 1) {
		index++;
		if (flagIsSurname == 0) flagIsSurname = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) == 1) {
		index++;
		if (flagIsName == 0) flagIsName = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) == 1) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (string[index] == '.') {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (string[index] == '.') {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	if (IsDigit(string[index])) {
		index++;
		flagIsBirthday++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (IsAlpha(string[index])) {
		if (string[index] != 'M' && string[index] != 'W') {
			return INCORRECT_DATA;
		}
		flagIsGender = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index])) {
		index++;
		if (flagIsSalary == 0) flagIsSalary = 1;
	}
	if (string[index] == '.') index++;
	while (IsDigit(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0') {
		return INCORRECT_DATA;
	}
	if (!(flagIsSurname && flagIsName && (flagIsBirthday == 10) && flagIsGender && flagIsSalary)) {
		return INCORRECT_DATA;
	}
	return NORMAL;
}

statusCode GetSurnameFromString(const char* string, char** surname) {
	*surname = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*surname == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) == 1) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* tempSurname = (char*)realloc(*surname, sizeof(char) * totalLength);
			if (tempSurname == NULL) {
				free(*surname);
				return MEMORY_ALLOCATION_ERROR;
			}
			*surname = tempSurname;
		}
		(*surname)[currentLength++] = string[index++];
	}
	(*surname)[currentLength] = '\0';
	return NORMAL;
}

statusCode GetNameFromString(const char* string, char** name) {
	*name = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*name == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* tempName = (char*)realloc(*name, sizeof(char) * totalLength);
			if (tempName == NULL) {
				free(*name);
				return MEMORY_ALLOCATION_ERROR;
			}
			*name = tempName;
		}
		(*name)[currentLength++] = string[index++];
	}
	(*name)[currentLength] = '\0';
	return NORMAL;
}

statusCode GetPatronymicFromString(const char* string, char** patronymic) {
	*patronymic = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*patronymic == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (IsDigit(string[index])) {
		(*patronymic)[currentLength] = '\0';
		return NORMAL;
	}
	while (IsAlpha(string[index])) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* tempPatronymic = (char*)realloc(*patronymic, sizeof(char) * totalLength);
			if (tempPatronymic == NULL) {
				free(*patronymic);
				return MEMORY_ALLOCATION_ERROR;
			}
			*patronymic = tempPatronymic;
		}
		(*patronymic)[currentLength++] = string[index++];
	}
	(*patronymic)[currentLength] = '\0';
	return NORMAL;
}

void GetBirthdayFromString(const char* string, int* day, int* month, int* year) {
	int index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	*day = (string[index] - '0') * 10 + string[index + 1] - '0';
	index += 3;
	*month = (string[index] - '0') * 10 + string[index + 1] - '0';
	index += 3;
	*year = (string[index] - '0') * 1000 + (string[index + 1] - '0') * 100 + (string[index + 2] - '0') * 10 +
	        string[index + 3] - '0';
}

void GetGenderFromString(const char* string, gender* current) {
	int index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index]) || string[index] == '.') index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == 'M') {
		*current = GENDER_MAN;
	} else if (string[index] == 'W') {
		*current = GENDER_WOMAN;
	}
}

statusCode GetSalaryFromString(const char* string, double* salary) {
	char* numberInString = (char*)malloc(sizeof(char) * START_LENGTH_NUMBER);
	if (numberInString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_NUMBER;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index]) || string[index] == '.') index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index]) || string[index] == '.') {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* tempString = (char*)realloc(numberInString, sizeof(char) * totalLength);
			if (tempString == NULL) {
				free(numberInString);
				return MEMORY_ALLOCATION_ERROR;
			}
			numberInString = tempString;
		}
		numberInString[currentLength++] = string[index++];
	}
	numberInString[currentLength] = '\0';
	sscanf(numberInString, "%lf", salary);
	free(numberInString);
	return NORMAL;
}

statusCode IsValidBirthday(int day, int month, int year) {
	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
		return INCORRECT_DATA;
	}
	int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
		if (day > 29) {
			return INCORRECT_DATA;
		}
	} else {
		if (day > daysInMonth[month - 1]) {
			return INCORRECT_DATA;
		}
	}
	struct tm birthday = {0};
	birthday.tm_year = year - 1900;
	birthday.tm_mon = month - 1;
	birthday.tm_mday = day;
	time_t result = mktime(&birthday);
	if (result == -1) {
		return INCORRECT_DATA;
	}
	time_t currentTime = time(NULL);
	if (difftime(currentTime, result) < 0) {
		return INCORRECT_DATA;
	}
	return NORMAL;
}

void FreeLiverData(Liver** liverData, int amountLivers) {
	for (int i = 0; i < amountLivers; i++) {
		DestroyString(&(*liverData)[i].surname);
		DestroyString(&(*liverData)[i].name);
		DestroyString(&(*liverData)[i].patronymic);
	}
	free(*liverData);
}

int CompareLiversDecrease(const void* first, const void* second) {
	double epsilon = 0.00000000000000000000000001;
	Liver* firstLiver = (Liver*)first;
	Liver* secondLiver = (Liver*)second;
	if (firstLiver->yearBirthday < secondLiver->yearBirthday) {
		return -1;
	} else if (firstLiver->yearBirthday > secondLiver->yearBirthday) {
		return 1;
	}
	if (firstLiver->monthBirthday < secondLiver->monthBirthday) {
		return -1;
	} else if (firstLiver->monthBirthday > secondLiver->monthBirthday) {
		return 1;
	}
	if (firstLiver->dayBirthday < secondLiver->dayBirthday) {
		return -1;
	} else if (firstLiver->dayBirthday > secondLiver->dayBirthday) {
		return 1;
	}
	int resultComparing = OrderString(&firstLiver->surname, &secondLiver->surname);
	if (resultComparing != 0) {
		return resultComparing;
	}
	resultComparing = OrderString(&firstLiver->name, &secondLiver->name);
	if (resultComparing != 0) {
		return resultComparing;
	}
	resultComparing = OrderString(&firstLiver->patronymic, &secondLiver->patronymic);
	if (resultComparing != 0) {
		return resultComparing;
	}
	if (firstLiver->salary - secondLiver->salary > epsilon) {
		return 1;
	} else if (firstLiver->salary - secondLiver->salary < -epsilon) {
		return -1;
	}
	return 0;
}


int CompareLiversIncrease(const void* first, const void* second) { return -CompareLiversDecrease(first, second); }


LiverNode* CreateLiverNode(String* surname, String* name, String* patronymic, int day, int month, int year,
                           double salary, int liverID, gender liverGender) {
	LiverNode* ptr = (LiverNode*)malloc(sizeof(LiverNode));
	if (ptr == NULL) {
		return NULL;
	}
	ptr->LiverID = liverID;
	ptr->next = NULL;
	ptr->currentLiver.dayBirthday = day;
	ptr->currentLiver.monthBirthday = month;
	ptr->currentLiver.yearBirthday = year;
	ptr->currentLiver.genderLiver = liverGender;
	ptr->currentLiver.name.symbols = NULL;
	ptr->currentLiver.surname.symbols = NULL;
	ptr->currentLiver.patronymic.symbols = NULL;
	CopyStringsOld(&(*ptr).currentLiver.surname, surname);
	CopyStringsOld(&(*ptr).currentLiver.name, name);
	CopyStringsOld(&(*ptr).currentLiver.patronymic, patronymic);
	ptr->currentLiver.salary = salary;
	return ptr;
}

void InsertLiverNode(LiverNode* previousNode, LiverNode* newNode) {
	newNode->next = previousNode->next;
	previousNode->next = newNode;
}

void PushLiverNode(LiverNode** head, LiverNode* newNode) {
	LiverNode* ptr = *head;
	LiverNode* previous = *head;
	while (ptr->currentLiver.yearBirthday > newNode ->currentLiver.yearBirthday) {
		previous = ptr;
		ptr = ptr -> next;
	}
	while (ptr->currentLiver.monthBirthday > newNode ->currentLiver.monthBirthday) {
		previous = ptr;
		ptr = ptr -> next;
	}
	while (ptr->currentLiver.dayBirthday > newNode ->currentLiver.dayBirthday) {
		previous = ptr;
		ptr = ptr -> next;
	}
	InsertLiverNode(previous, newNode);
}

void DeleteLiverNode(LiverNode* previousNode) {
	LiverNode* ptr = previousNode->next;
	previousNode->next = ptr->next;
	DestroyString(&ptr->currentLiver.name);
	DestroyString(&ptr->currentLiver.surname);
	DestroyString(&ptr->currentLiver.patronymic);
	free(ptr);
}

void DeleteLiverByID(LiverNode* head, int id) {
	LiverNode* previous = head;
	LiverNode* ptr = head;
	while (ptr != NULL && ptr -> LiverID != id) {
		previous = ptr;
		ptr = ptr -> next;
	}
	DeleteLiverNode(previous);
}

options GetCurrentOption() {
	char* currentOption = (char*)malloc(sizeof(char) * (MAX_LENGTH_OPTION + 1));
	if (currentOption == NULL) {
		return OPTION_NONE;
	}
	int index = 0;
	int c = getc(stdin);
	int flagIsWrong = 0;
	while (c == ' ' || c == '\t') {
		c = getc(stdin);
	}
	while (c != '\n') {
		if (index < MAX_LENGTH_OPTION && c != ' ' && c != '\t') {
			currentOption[index++] = (char)c;
		}
		c = getc(stdin);
		if (index >= MAX_LENGTH_OPTION && !(c == ' ' || c == '\t' || c == '\0' || c == '\n')) flagIsWrong = 1;
	}
	if (flagIsWrong) {
		free(currentOption);
		return OPTION_NONE;
	}
	currentOption[index] = '\0';
	String current = CreateString(currentOption);
	String strMenu = CreateString("menu");
	String strEnd = CreateString("end");
	String strFind = CreateString("find");
	String strEdit = CreateString("edit");
	String strDelete = CreateString("delete");
	String strAdd = CreateString("add");
	String strWrite = CreateString("writetofile");
	String strCancel = CreateString("undo");
	options result = OPTION_NONE;
	if (EqualOptions(&current, &strMenu) == 1) {
		result = OPTION_MENU;
	} else if (EqualOptions(&current, &strEnd) == 1) {
		result = OPTION_END;
	} else if (EqualOptions(&current, &strFind) == 1) {
		result = OPTION_FIND;
	} else if (EqualOptions(&current, &strEdit) == 1) {
		result = OPTION_EDIT;
	} else if (EqualOptions(&current, &strDelete) == 1) {
		result = OPTION_DELETE;
	} else if (EqualOptions(&current, &strAdd) == 1) {
		result = OPTION_ADD;
	} else if (EqualOptions(&current, &strWrite) == 1) {
		result = OPTION_WRITE_TO_FILE;
	} else if (EqualOptions(&current, &strCancel) == 1) {
		result = OPTION_CANCEL;
	}
	free(currentOption);
	DestroyString(&current);
	DestroyString(&strMenu);
	DestroyString(&strEnd);
	DestroyString(&strFind);
	DestroyString(&strEdit);
	DestroyString(&strDelete);
	DestroyString(&strAdd);
	DestroyString(&strWrite);
	DestroyString(&strCancel);
	return result;
}

void GetMenu() {
	printf("----------------------------------------------------------------------------------\n");
	printf("|--------------------------------------MENU--------------------------------------|\n");
	printf("| If you want to execute the option, then type the command specified after ':'   |\n");
	printf("|--------------------------------------------------------------------------------|\n");
	printf("| Find information about liver by some parameters: find                          |\n");
	printf("| Edit information about liver                   : edit                          |\n");
	printf("| Delete information about liver                 : delete                        |\n");
	printf("| Write information about all livers into file   : write to file                 |\n");
	printf("| Add information about new liver                : add                           |\n");
	printf("| Undo the previous action                       : undo                          |\n");
	printf("| Stop the program                               : end                           |\n");
	printf("|--------------------------------------------------------------------------------|\n");
	printf("| All other commands will be recognized as incorrect and the program will        |\n");
	printf("| require re-entry. Commands can be written in any register with any number of   |\n");
	printf("| spaces and tabs. Other delimiters are not supported and the command will be    |\n");
	printf("| recognized as incorrect.                                                       |\n");
	printf("---------------------------------------------------------------------------------\n");
}

void FindParameters() {
	printf("----------------------------------------------------------------------------------\n");
	printf("|--------------------------------------FIND--------------------------------------|\n");
	printf("| If you want to find by parameter, then type the command specified after ':'    |\n");
	printf("|--------------------------------------------------------------------------------|\n");
	printf("| Surname                                                 : surname              |\n");
	printf("| Name                                                    : name                 |\n");
	printf("| Patronymic                                              : patronymic           |\n");
	printf("| Gender                                                  : gender               |\n");
	printf("| Birthday                                                : birthday             |\n");
	printf("| Monthly revenue                                         : salary               |\n");
	printf("|--------------------------------------------------------------------------------|\n");
}

void ChangeParameters() {
	printf("----------------------------------------------------------------------------------\n");
	printf("|-------------------------------------CHANGE-------------------------------------|\n");
	printf("| If you want to change this parameter, then type the command specified after ':'|\n");
	printf("|--------------------------------------------------------------------------------|\n");
	printf("| Surname                                                 : surname              |\n");
	printf("| Name                                                    : name                 |\n");
	printf("| Patronymic                                              : patronymic           |\n");
	printf("| Gender                                                  : gender               |\n");
	printf("| Birthday                                                : birthday             |\n");
	printf("| Monthly revenue                                         : salary               |\n");
	printf("|--------------------------------------------------------------------------------|\n");
}

statusCode GetString(char** currentString) {
	*currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(*currentString, sizeof(char) * (currentLength + 1));
			if (tempString == NULL) {
				free(*currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			*currentString = tempString;
		}
		(*currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(*currentString)[index] = '\0';
	return NORMAL;
}

void DeleteLiverList(LiverNode** head) {
	while (*head != NULL) {
		LiverNode* ptr = *head;
		*head = (*head)->next;
		DestroyString(&ptr->currentLiver.surname);
		DestroyString(&ptr->currentLiver.name);
		DestroyString(&ptr->currentLiver.patronymic);
		free(ptr);
	}
}

answer GetAnswer() {
	answer currentAnswer = ANSWER_UNKNOWN;
	char* string;
	statusCode resultReading = GetString(&string);
	if (resultReading == MEMORY_ALLOCATION_ERROR) {
		return ANSWER_UNKNOWN;
	}
	String answerString = CreateString(string);
	String positiveAnswer = CreateString("yes");
	String negativeAnswer = CreateString("no");
	if (EqualOptions(&answerString, &positiveAnswer) == 1) {
		currentAnswer = ANSWER_YES;
	} else if (EqualOptions(&answerString, &negativeAnswer) == 1) {
		currentAnswer = ANSWER_NO;
	}
	free(string);
	DestroyString(&answerString);
	DestroyString(&positiveAnswer);
	DestroyString(&negativeAnswer);
	return currentAnswer;
}

answer GetParameterFind() {
	answer currentAnswer = ANSWER_UNKNOWN;
	char* string;
	statusCode resultReading = GetString(&string);
	if (resultReading == MEMORY_ALLOCATION_ERROR) {
		return ANSWER_UNKNOWN;
	}
	String answerString = CreateString(string);
	String surnameString = CreateString("surname");
	String nameString = CreateString("name");
	String patrString = CreateString("patronymic");
	String birthdayString = CreateString("birthday");
	String salaryString = CreateString("salary");
	String genderString = CreateString("gender");
	if (EqualOptions(&answerString, &surnameString) == 1) {
		currentAnswer = ANSWER_SURNAME;
	} else if (EqualOptions(&answerString, &nameString) == 1) {
		currentAnswer = ANSWER_NAME;
	} else if (EqualOptions(&answerString, &patrString) == 1) {
		currentAnswer = ANSWER_PATRONYMIC;
	} else if (EqualOptions(&answerString, &birthdayString) == 1) {
		currentAnswer = ANSWER_BIRTHDAY;
	} else if (EqualOptions(&answerString, &salaryString) == 1) {
		currentAnswer = ANSWER_SALARY;
	} else if (EqualOptions(&answerString, &genderString) == 1) {
		currentAnswer = ANSWER_GENDER;
	}
	free(string);
	DestroyString(&answerString);
	DestroyString(&surnameString);
	DestroyString(&nameString);
	DestroyString(&patrString);
	DestroyString(&birthdayString);
	DestroyString(&salaryString);
	DestroyString(&genderString);
	return currentAnswer;
}

statusCode GetSurnameFromConsole(String* surname) {
	char* currentString;
	statusCode status = GetString(&currentString);
	if (status == MEMORY_ALLOCATION_ERROR) {
		return MEMORY_ALLOCATION_ERROR;
	}
	if (IsValidName(currentString) == INCORRECT_DATA) {
		free(currentString);
		return INCORRECT_DATA;
	}
	*surname = CreateString(currentString);
	free(currentString);
	return NORMAL;
}

statusCode GettingSurname(String* surname) {
	statusCode status = INCORRECT_DATA;
	int flagIsFirst = 1;
	while (status == INCORRECT_DATA) {
		if (!flagIsFirst) {
			printf(
			    "Incorrect surname. It can contain only latin letters in any case.\n"
			    "Try again: ");
		}
		flagIsFirst = 0;
		status = GetSurnameFromConsole(surname);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
	}
	return NORMAL;
}

statusCode GettingName(String* name) {
	statusCode status = INCORRECT_DATA;
	int flagIsFirst = 1;
	while (status == INCORRECT_DATA) {
		if (!flagIsFirst) {
			printf(
			    "Incorrect name. It can contain only latin letters in any case.\n"
			    "Try again: ");
		}
		flagIsFirst = 0;
		status = GetSurnameFromConsole(name);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
	}
	return NORMAL;
}

statusCode GetPatronymicFromConsole(String* surname) {
	char* currentString;
	statusCode status = GetString(&currentString);
	if (status == MEMORY_ALLOCATION_ERROR) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < StringLength(currentString); i++) {
		if (!(IsAlpha(currentString[i]) || currentString[i] == ' ')) {
			free(currentString);
			return INCORRECT_DATA;
		}
	}
	*surname = CreateString(currentString);
	free(currentString);
	return NORMAL;
}

statusCode GettingPatronymic(String* patronymic) {
	statusCode status = INCORRECT_DATA;
	int flagIsFirst = 1;
	while (status == INCORRECT_DATA) {
		if (!flagIsFirst) {
			printf(
			    "Incorrect patronymic. It can contain only latin letters\n"
			    "in any case or be empty.\nTry again: ");
		}
		flagIsFirst = 0;
		status = GetPatronymicFromConsole(patronymic);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
	}
	return NORMAL;
}


statusCode IsValidName(char* name) {
	size_t length = StringLength(name);
	if (length == 0) {
		return INCORRECT_DATA;
	}
	int countWhiteSpace = 0;
	for (int i = 0; i < length; i++) {
		if (name[i] == ' ') countWhiteSpace++;
	}
	if (countWhiteSpace == length) {
		return INCORRECT_DATA;
	}
	for (int i = 0; i < length; i++) {
		if (!(name[i] >= 'a' && name[i] <= 'z' || name[i] >= 'A' && name[i] <= 'Z' || name[i] == ' ')) {
			return INCORRECT_DATA;
		}
	}
	return NORMAL;
}


statusCode IsValidFileName(const char* string) {
	int index = 0;
	int flagGoodEnding = 0;
	int flagGoodStart = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) || IsDigit(string[index]) || string[index] == '_') {
		index++;
		if (flagGoodStart == 0) flagGoodStart = 1;
	}
	if (string[index] == '.' && string[index + 1] == 't' && string[index + 2] == 'x' && string[index + 3] == 't') {
		index += 4;
		flagGoodEnding = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0' || !(flagGoodStart && flagGoodEnding)) return INCORRECT_DATA;
	return NORMAL;
}


statusCode ConsoleReadLatin(char** currentString) {
	*currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(*currentString, sizeof(char) * currentLength);
			if (tempString == NULL) {
				free(*currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			*currentString = tempString;
		}
		(*currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(*currentString)[index] = '\0';
	for (int i = 0; i < index - 1; i++) {
		if (!IsAlpha((*currentString)[i])) {
			free(*currentString);
			return INCORRECT_DATA;
		}
	}
	return NORMAL;
}


statusCode ConsoleReadStr(char** currentString) {
	*currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(*currentString, sizeof(char) * currentLength);
			if (tempString == NULL) {
				free(*currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			*currentString = tempString;
		}
		(*currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(*currentString)[index] = '\0';
	for (int i = 0; i < index; i++) {
		if (!(IsAlpha((*currentString)[i]) || (*currentString)[i] == ' ')) {
			free(*currentString);
			return INCORRECT_DATA;
		}
	}
	return NORMAL;
}


statusCode ConsoleReadBirthday(int* day, int* month, int* year) {
	char* currentString;
	currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(currentString, sizeof(char) * currentLength);
			if (tempString == NULL) {
				free(currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			currentString = tempString;
		}
		(currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(currentString)[index] = '\0';
	if (!(IsDigit((currentString)[0]) && IsDigit((currentString)[1]) && (currentString)[2] == '.' &&
	      IsDigit((currentString)[3]) && IsDigit((currentString)[4]) && (currentString)[5] == '.' &&
	      IsDigit((currentString)[6]) && IsDigit((currentString)[7]) && IsDigit((currentString)[8]) &&
	      IsDigit((currentString)[9]))) {
		free(currentString);
		return INCORRECT_DATA;
	}
	for (int i = 10; i < index; i++) {
		if (!((currentString)[i] == ' ' || (currentString)[i] == '\t')) {
			free(currentString);
			return INCORRECT_DATA;
		}
	}
	*day = (currentString[0] - '0') * 10 + currentString[1] - '0';
	*month = (currentString[3] - '0') * 10 + currentString[4] - '0';
	*year = (currentString[6] - '0') * 1000 + (currentString[7] - '0') * 100 + (currentString[8] - '0') * 10 +
	        currentString[9] - '0';
	free(currentString);
	if (IsValidBirthday(*day, *month, *year) == INCORRECT_DATA) {
		return INCORRECT_DATA;
	}
	return NORMAL;
}


statusCode GettingBirthday(int* day, int* month, int* year) {
	statusCode status = INCORRECT_DATA;
	int flagIsFirst = 1;
	while (status == INCORRECT_DATA) {
		if (!flagIsFirst) {
			printf("Incorrect birthday. It should be written like: dd.MM.yyyy\n"
			    "and can't be later today. Try again: ");
		}
		flagIsFirst = 0;
		status = ConsoleReadBirthday(day, month, year);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
	}
	return NORMAL;
}


statusCode GettingSalary(double* salary) {
	statusCode status = INCORRECT_DATA;
	int flagIsFirst = 1;
	while (status == INCORRECT_DATA) {
		if (!flagIsFirst) {
			printf("Incorrect monthly revenue. It should be non-negative real number\n"
			    "(separator is dot). Try again: ");
		}
		flagIsFirst = 0;
		status = ConsoleReadDouble(salary);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
	}
	return NORMAL;
}


statusCode GettingGender(gender* current) {
	statusCode status = INCORRECT_DATA;
	int flagIsFirst = 1;
	char* string;
	while (status == INCORRECT_DATA) {
		if (!flagIsFirst) {
			printf("Incorrect data. Type M for man and W for woman.\n"
			    "Try again: ");
		}
		flagIsFirst = 0;
		status = GetString(&string);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
		if (StringLength(string) == 0) {
			free(string);
			status = INCORRECT_DATA;
		}
		else if (StringLength(string) != 1 || !(string[0] == 'W' || string[0] == 'M')) {
			free(string);
			status = INCORRECT_DATA;
		}
		else if (string[0] == 'M' || string[0] == 'm') {
			free(string);
			*current = GENDER_MAN;
			return NORMAL;
		} else if (string[0] == 'W' || string[0] == 'w') {
			free(string);
			*current = GENDER_WOMAN;
			return NORMAL;
		}
	}
	return NORMAL;
}


void PrintNameFound(char* resultScan, LiverNode* head) {
	String stringScan = CreateString(resultScan);
	if (StringLength(stringScan.symbols) == 0) {
		return;
	}
	LiverNode* ptr = head;
	int count = 0;
	while (ptr != NULL) {
		if (EqualOptions(&ptr->currentLiver.name, &stringScan)) {
			count++;
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
			printf("Liver's ID: %d\n", ptr->LiverID);
			printf("Surname: %s\n", ptr->currentLiver.surname.symbols);
			printf("Name: %s\n", ptr->currentLiver.name.symbols);
			printf("Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
			if (ptr ->currentLiver.genderLiver == GENDER_MAN) {
				printf("Gender: MAN\n");
			} else {
				printf("Gender: WOMAN\n");
			}
			printf("Birthday: %02d:%02d:%02d\n", ptr->currentLiver.dayBirthday, ptr->currentLiver.monthBirthday,
			       ptr->currentLiver.yearBirthday);
			printf("Monthly revenue: %lf\n", ptr->currentLiver.salary);
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
		}
		ptr = ptr->next;
	}
	if (count == 0) {
		printf("---------------------------------------------------------------------------------\n");
		printf("There is no livers with this name.\n");
		printf("---------------------------------------------------------------------------------\n");
	}
	DestroyString(&stringScan);
}

int PrintSurnameFound(char* resultScan, LiverNode* head) {
	String stringScan = CreateString(resultScan);
	if (StringLength(stringScan.symbols) == 0) {
		return 0;
	}
	LiverNode* ptr = head;
	int count = 0;
	while (ptr != NULL) {
		if (EqualOptions(&ptr->currentLiver.surname, &stringScan)) {
			count++;
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
			printf("Liver's ID: %d\n", ptr->LiverID);
			printf("Surname: %s\n", ptr->currentLiver.surname.symbols);
			printf("Name: %s\n", ptr->currentLiver.name.symbols);
			printf("Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
			if (ptr -> currentLiver.genderLiver == GENDER_MAN) {
				printf("Gender: MAN\n");
			} else
				printf("Gender: WOMAN\n");
			printf("Birthday: %02d:%02d:%d\n", ptr->currentLiver.dayBirthday, ptr->currentLiver.monthBirthday,
			       ptr->currentLiver.yearBirthday);
			printf("Monthly revenue: %lf\n", ptr->currentLiver.salary);
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
		}
		ptr = ptr->next;
	}
	if (count == 0) {
		printf("---------------------------------------------------------------------------------\n");
		printf("There is no livers with this surname.\n");
		printf("---------------------------------------------------------------------------------\n");
	}
	DestroyString(&stringScan);
	return count;
}


void PrintPatronymicFound(char* resultScan, LiverNode* head) {
	String stringScan = CreateString(resultScan);
	LiverNode* ptr = head;
	int count = 0;
	while (ptr != NULL) {
		if (EqualOptions(&ptr->currentLiver.patronymic, &stringScan)) {
			count++;
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
			printf("Liver's ID: %d\n", ptr->LiverID);
			printf("Surname: %s\n", ptr->currentLiver.surname.symbols);
			printf("Name: %s\n", ptr->currentLiver.name.symbols);
			printf("Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
			if (ptr->currentLiver.genderLiver == GENDER_MAN) {
				printf("Gender: MAN\n");
			} else {
				printf("Gender: WOMAN\n");
			}
			printf("Birthday: %02d:%02d:%d\n", ptr->currentLiver.dayBirthday, ptr->currentLiver.monthBirthday,
			       ptr->currentLiver.yearBirthday);
			printf("Monthly revenue: %lf\n", ptr->currentLiver.salary);
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
		}
		ptr = ptr->next;
	}
	if (count == 0) {
		printf("---------------------------------------------------------------------------------\n");
		printf("There is no livers with this patronymic.\n");
		printf("---------------------------------------------------------------------------------\n");
	}
	DestroyString(&stringScan);
}


void PrintGenderFound(gender current, LiverNode* head) {
	LiverNode* ptr = head;
	int count = 0;
	while (ptr != NULL) {
		if (ptr->currentLiver.genderLiver == current) {
			count++;
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
			printf("Liver's ID: %d\n", ptr->LiverID);
			printf("Surname: %s\n", ptr->currentLiver.surname.symbols);
			printf("Name: %s\n", ptr->currentLiver.name.symbols);
			printf("Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
			if (ptr -> currentLiver.genderLiver ==  GENDER_MAN) {
				printf("Gender: MAN\n");
			} else {
				printf("Gender: WOMAN\n");
			}
			printf("Birthday: %02d:%02d:%d\n", ptr->currentLiver.dayBirthday, ptr->currentLiver.monthBirthday,
			       ptr->currentLiver.yearBirthday);
			printf("Monthly revenue: %lf\n", ptr->currentLiver.salary);
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
		}
		ptr = ptr->next;
	}
	if (count == 0) {
		printf("---------------------------------------------------------------------------------\n");
		printf("There is no livers with this gender.\n");
		printf("---------------------------------------------------------------------------------\n");
	}
}


void PrintBirthdayFound(int day, int month, int year, LiverNode* head) {
	LiverNode* ptr = head;
	int count = 0;
	while (ptr != NULL) {
		if (ptr->currentLiver.dayBirthday == day && ptr->currentLiver.monthBirthday == month &&
		    ptr->currentLiver.yearBirthday == year) {
			count++;
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
			printf("Liver's ID: %d\n", ptr->LiverID);
			printf("Surname: %s\n", ptr->currentLiver.surname.symbols);
			printf("Name: %s\n", ptr->currentLiver.name.symbols);
			printf("Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
			if (ptr -> currentLiver.genderLiver == GENDER_MAN) {
				printf("Gender: MAN\n");
			} else
				printf("Gender: WOMAN\n");
			printf("Birthday: %02d:%02d:%d\n", ptr->currentLiver.dayBirthday, ptr->currentLiver.monthBirthday,
			       ptr->currentLiver.yearBirthday);
			printf("Monthly revenue: %lf\n", ptr->currentLiver.salary);
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
		}
		ptr = ptr->next;
	}
	if (count == 0) {
		printf("---------------------------------------------------------------------------------\n");
		printf("There is no livers with this birthday.\n");
		printf("---------------------------------------------------------------------------------\n");
	}
}


statusCode ConsoleReadDouble(double* result) {
	char* currentString;
	currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(currentString, sizeof(char) * currentLength);
			if (tempString == NULL) {
				free(currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			currentString = tempString;
		}
		(currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(currentString)[index] = '\0';
	int countDots = 0;
	for (int i = 0; i < index; i++) {
		if (currentString[i] == '.') {
			countDots++;
			if (countDots > 1) {
				free(currentString);
				return INCORRECT_DATA;
			}
		} else if (!IsDigit(currentString[i])) {
			free(currentString);
			return INCORRECT_DATA;
		}
	}
	if (index == 0) {
		free(currentString);
		return INCORRECT_DATA;
	}
	sscanf(currentString, "%lf", result);
	free(currentString);
	return NORMAL;
}


statusCode ConsoleReadInteger(int* result) {
	char* currentString;
	currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(currentString, sizeof(char) * currentLength);
			if (tempString == NULL) {
				free(currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			currentString = tempString;
		}
		(currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(currentString)[index] = '\0';
	for (int i = 0; i < index; i++) {
		if (!IsDigit(currentString[i])) {
			free(currentString);
			return INCORRECT_DATA;
		}
	}
	sscanf(currentString, "%d", result);
	free(currentString);
	return NORMAL;
}


void PrintSalaryFound(double salary, double epsilon, LiverNode* head) {
	LiverNode* ptr = head;
	int count = 0;
	while (ptr != NULL) {
		if (ptr->currentLiver.salary - salary < epsilon && ptr->currentLiver.salary - salary > -epsilon) {
			count++;
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
			printf("Liver's ID: %d\n", ptr->LiverID);
			printf("Surname: %s\n", ptr->currentLiver.surname.symbols);
			printf("Name: %s\n", ptr->currentLiver.name.symbols);
			printf("Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
			if (ptr -> currentLiver.genderLiver == GENDER_MAN) {
				printf("Gender: MAN\n");
			} else {
				printf("Gender: WOMAN\n");
			}
			printf("Birthday: %02d:%02d:%d\n", ptr->currentLiver.dayBirthday, ptr->currentLiver.monthBirthday,
			       ptr->currentLiver.yearBirthday);
			printf("Monthly revenue: %lf\n", ptr->currentLiver.salary);
			printf(
			    "---------------------------------------------------------------------------------"
			    "\n");
		}
		ptr = ptr->next;
	}
	if (count == 0) {
		printf("---------------------------------------------------------------------------------\n");
		printf("There is no livers with this salary.\n");
		printf("---------------------------------------------------------------------------------\n");
	}
}

void FreeEditReminders(EditReminder ** reminders, int amount) {
	for (int i = 0; i < amount; i++) {
		if ((*reminders)[i].changedOption == ANSWER_NAME || (*reminders)[i].changedOption == ANSWER_PATRONYMIC ||
		    (*reminders)[i].changedOption == ANSWER_SURNAME) {
			DestroyString(&(*reminders)[i].changedString);
		}
	}
	free(*reminders);
}
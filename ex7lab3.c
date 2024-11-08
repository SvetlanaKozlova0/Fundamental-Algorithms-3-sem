#include <stdio.h>
#include <stdlib.h>

#include "ex7lab3library.h"
int main() {
	Liver* liverData;
	int countLivers = 0;
	char* nameInputFile = "inputEX7.txt";
	statusCode status = FillArrayLivers(&liverData, &countLivers, nameInputFile);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case FILE_OPEN_ERROR:
			printf("An error while opening file.\n");
			return 1;
		case INCORRECT_DATA:
			printf("File contains incorrect data.\n");
			return 1;
	}
	double epsilon = 0.000000000000000000000001;
	qsort(liverData, countLivers, sizeof(Liver), CompareLiversIncrease);
	int currentID = 0;
	LiverNode* head = CreateLiverNode(&liverData[0].surname, &liverData[0].name, &liverData[0].patronymic,
	                                  liverData[0].dayBirthday, liverData[0].monthBirthday, liverData[0].yearBirthday,
	                                  liverData[0].salary, currentID, liverData[0].genderLiver);
	if (head == NULL) {
		FreeLiverData(&liverData, countLivers);
		printf("Memory allocation error.\n");
		return 1;
	}
	LiverNode* previous = head;
	for (int i = 1; i < countLivers; i++) {
		currentID++;
		LiverNode* next =
		    CreateLiverNode(&liverData[i].surname, &liverData[i].name, &liverData[i].patronymic,
		                    liverData[i].dayBirthday, liverData[i].monthBirthday, liverData[i].yearBirthday,
		                    liverData[i].salary, currentID, liverData[i].genderLiver);
		if (next == NULL) {
			FreeLiverData(&liverData, countLivers);
			DeleteLiverList(&head);
			printf("Memory allocation error.\n");
			return 1;
		}
		InsertLiverNode(previous, next);
		previous = next;
	}
	FreeLiverData(&liverData, countLivers);
	options currentOption = OPTION_NONE;
	answer currentAnswer = ANSWER_UNKNOWN;
	GetMenu();
	options* laterActions = (options*)malloc(sizeof(options) * START_AMOUNT_OPTIONS);
	if (laterActions == NULL) {
		DeleteLiverList(&head);
		printf("Memory allocation error.\n");
		return 1;
	}
	EditReminder* editReminders = (EditReminder*)malloc(sizeof(EditReminder) * START_AMOUNT_OPTIONS);
	if (editReminders == NULL) {
		DeleteLiverList(&head);
		free(laterActions);
		printf("Memory allocation error.\n");
		return 1;
	}
	int* addedID = (int*)malloc(sizeof(int) * START_AMOUNT_OPTIONS);
	if (addedID == NULL) {
		DeleteLiverList(&head);
		free(laterActions);
		free(editReminders);
		printf("Memory allocation error.\n");
		return 1;
	}
	int totalAddedID = START_AMOUNT_OPTIONS;
	int countAddedID = 0;
	int countAllOperations = 0;
	int totalOperationsLength = START_AMOUNT_OPTIONS;
	int countOperationsUndo = 0;
	int countEdits = 0;
	int totalEditsLength = START_AMOUNT_OPTIONS;
	Liver* deletedLivers = (Liver*)malloc(sizeof(Liver) * START_AMOUNT_OPTIONS);
	if (deletedLivers == NULL) {
		DeleteLiverList(&head);
		free(laterActions);
		free(editReminders);
		free(addedID);
		printf("Memory allocation error.\n");
		return 1;
	}
	int countDeletedLivers = 0;
	int totalLengthDeleted = START_AMOUNT_OPTIONS;
	while (currentOption != OPTION_END) {
		printf("Please, choose the option: ");
		currentOption = GetCurrentOption();
		printf("---------------------------------------------------------------------------------\n");
		switch (currentOption) {
			case OPTION_MENU:
				printf("You choose option 'MENU'\n");
				GetMenu();
				break;
			case OPTION_FIND:
				printf("You choose option 'FIND'. This option can find livers by parameters.\n");
				printf("Please, choose by which parameter do you want to search.\n");
				FindParameters();
				answer parameter = GetParameterFind();
				char* resultScan;
				switch (parameter) {
					case ANSWER_SURNAME:
						printf(
						    "You choose find by surname. Surname can contain only latin letters\n"
						    "and can't be empty. Please, enter the surname: ");
						String findString;
						status = GettingSurname(&findString);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							free(addedID);
							FreeEditReminders(&editReminders, countEdits);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						if (status == INCORRECT_DATA) {
							printf("Incorrect data. Restart the option.\n");
							printf(
							    "---------------------------------------------------------------------------------\n");
							break;
						}
						PrintSurnameFound(findString.symbols, head);
						DestroyString(&findString);
						break;
					case ANSWER_NAME:
						printf(
						    "You choose find by name. Name can contain only latin letters\n"
						    "and can't be empty. Please, enter the name: ");
						status = GettingName(&findString);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							free(addedID);
							FreeEditReminders(&editReminders, countEdits);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						if (status == INCORRECT_DATA) {
							printf("Incorrect data. Restart the option.\n");
							printf(
							    "---------------------------------------------------------------------------------\n");
							break;
						}
						PrintNameFound(findString.symbols, head);
						DestroyString(&findString);
						break;
					case ANSWER_PATRONYMIC:
						printf(
						    "You choose find by patronymic. Patronymic can contain only latin\n"
						    "letters or be empty.\n"
						    "Please, enter the patronymic: ");
						status = GettingPatronymic(&findString);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							FreeEditReminders(&editReminders, countEdits);
							return 1;
						}
						if (status == INCORRECT_DATA) {
							printf("Incorrect data. Restart the option.\n");
							printf(
							    "---------------------------------------------------------------------------------\n");
							break;
						}
						PrintPatronymicFound(findString.symbols, head);
						DestroyString(&findString);
						break;
					case ANSWER_GENDER:
						printf("You choose find by gender. Gender is M for man and W for woman.\n");
						printf("Please, enter the gender: ");
						gender current;
						status = GettingGender(&current);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							free(addedID);
							FreeEditReminders(&editReminders, countEdits);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						PrintGenderFound(current, head);
						break;
					case ANSWER_BIRTHDAY:
						printf(
						    "You choose find by birthday. The date of birth should be\n"
						    "written as dd:MM:yyyy and can't be later today\n"
						    "Please, enter the birthday: ");
						int day, month, year;
						status = GettingBirthday(&day, &month, &year);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							free(addedID);
							FreeEditReminders(&editReminders, countEdits);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						if (status == INCORRECT_DATA) {
							printf("Incorrect data. Restart the option.\n");
							printf(
							    "---------------------------------------------------------------------------------\n");
							break;
						}
						PrintBirthdayFound(day, month, year, head);
						break;
					case ANSWER_SALARY:
						printf(
						    "You choose find by monthly revenue. Monthly revenue can\n"
						    "be a non-negative real number (dot separator).\n"
						    "Please, enter the monthly revenue: ");
						double salary;
						status = GettingSalary(&salary);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							FreeEditReminders(&editReminders, countEdits);
							return 1;
						}
						if (status == INCORRECT_DATA) {
							printf(
							    "Incorrect data. Monthly revenue can be only a non-negative\n"
							    "real number. Restart the option.\n");
							printf(
							    "---------------------------------------------------------------------------------\n");
							break;
						}
						if (salary <= -epsilon) {
							printf("Monthly revenue can't be negative. Restart the option.\n");
							printf(
							    "---------------------------------------------------------------------------------\n");
							break;
						}
						PrintSalaryFound(salary, epsilon, head);
						break;
					case ANSWER_UNKNOWN:
						printf("Unknown answer. Try again.\n");
						break;
				}
				break;
			case OPTION_EDIT:
				printf(
				    "You choose option 'EDIT'. You can use this option to edit information\n"
				    "about a liver. Do you want to continue? Type yes or no.\n");
				currentAnswer = GetAnswer();
				if (currentAnswer == ANSWER_NO) {
					printf("You choose no. Select another option or try again.\n");
					break;
				} else if (currentAnswer != ANSWER_YES) {
					printf("Unknown answer. Select another option or try again.\n");
					break;
				}
				printf("---------------------------------------------------------------------------------\n");
				printf(
				    "You choose yes. Now you will be asked to enter the surname\n"
				    "of the liver, after which the program will give out all livers with\n"
				    "this surname. Select the desired ID and continue.\n");
				printf("---------------------------------------------------------------------------------\n");
				printf("Please, enter the surname: ");
				String surname;
				status = GettingSurname(&surname);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					FreeEditReminders(&editReminders, countEdits);
					return 1;
				}
				printf("Surname is %s\n", surname.symbols);
				int res = PrintSurnameFound(surname.symbols, head);
				DestroyString(&surname);
				if (res == 0) {
					printf("There is no livers with this surname. Nothing to edit.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				printf("Please, enter the selected ID: ");
				int id;
				status = ConsoleReadInteger(&id);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					FreeEditReminders(&editReminders, countEdits);
					return 1;
				}
				if (status == INCORRECT_DATA) {
					printf("ID contains incorrect symbols. Restart the option.\n");
					break;
				}
				printf("ID is %d. Do you want to continue? Type yes or no.\n", id);
				currentAnswer = GetAnswer();
				if (currentAnswer == ANSWER_NO) {
					printf("You choose no. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				} else if (currentAnswer != ANSWER_YES) {
					printf("Unknown answer. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				printf("Please, choose with parameter you want to change.\n");
				LiverNode* ptrEdit = head;
				while (ptrEdit->LiverID != id && ptrEdit->next != NULL) {
					ptrEdit = ptrEdit->next;
				}
				if (ptrEdit->next == NULL) {
					printf("There is no liver with that ID.\n");
					break;
				}
				ChangeParameters();
				parameter = GetParameterFind();
				if (countAllOperations + 1 > totalOperationsLength) {
					totalOperationsLength *= 2;
					options* tempOptions = (options*)realloc(laterActions, sizeof(options) * totalOperationsLength);
					if (tempOptions == NULL) {
						DeleteLiverList(&head);
						free(laterActions);
						FreeEditReminders(&editReminders, countEdits);
						FreeLiverData(&deletedLivers, countDeletedLivers);
						printf("Memory allocation error.\n");
						free(addedID);
						return 1;
					}
					laterActions = tempOptions;
				}
				if (parameter == ANSWER_UNKNOWN) {
					printf("Unknown answer. Restart the option.\n");
					break;
				}
				laterActions[countAllOperations++] = OPTION_EDIT;
				if (countAllOperations % 2 == 0) {
					countOperationsUndo++;
				}
				if (countEdits + 1 > totalEditsLength) {
					totalEditsLength *= 2;
					EditReminder* tempReminders =
					    (EditReminder*)realloc(editReminders, sizeof(EditReminder) * totalEditsLength);
					if (tempReminders == NULL) {
						DeleteLiverList(&head);
						free(laterActions);
						FreeEditReminders(&editReminders, countEdits);
						printf("Memory allocation error.\n");
						free(addedID);
						FreeLiverData(&deletedLivers, countDeletedLivers);
						return 1;
					}
					editReminders = tempReminders;
				}
				switch (parameter) {
					case ANSWER_SURNAME:
						printf(
						    "You choose change surname. Surname can contain only latin letters\n"
						    "and can't be empty.\n");
						printf("Please, enter new surname: ");
						status = GettingSurname(&surname);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							FreeEditReminders(&editReminders, countEdits);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						editReminders[countEdits].changedOption = ANSWER_SURNAME;
						editReminders[countEdits].changedString.symbols = NULL;
						CopyStringsOld(&editReminders[countEdits].changedString, &ptrEdit->currentLiver.surname);
						editReminders[countEdits].id = id;
						countEdits++;
						printf("Surname is %s\n", surname.symbols);
						CopyStringsOld(&ptrEdit->currentLiver.surname, &surname);
						printf("Information was edit successfully.\n");
						printf("---------------------------------------------------------------------------------\n");
						DestroyString(&surname);
						break;
					case ANSWER_NAME:
						printf(
						    "You choose change name. Name can contain only latin letters\n"
						    "and can't be empty.\n");
						printf("Please, enter new name: ");
						String curName;
						status = GettingName(&curName);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							FreeEditReminders(&editReminders, countEdits);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						editReminders[countEdits].changedOption = ANSWER_NAME;
						editReminders[countEdits].changedString.symbols = NULL;
						CopyStringsOld(&editReminders[countEdits].changedString, &ptrEdit->currentLiver.name);
						editReminders[countEdits].id = id;
						countEdits++;
						printf("Name is %s\n", curName.symbols);
						CopyStringsOld(&ptrEdit->currentLiver.name, &curName);
						printf("Information was edit successfully.\n");
						printf("---------------------------------------------------------------------------------\n");
						DestroyString(&curName);
						break;
					case ANSWER_PATRONYMIC:
						printf(
						    "You choose change patronymic. Patronymic can contain latin letters\n"
						    "or be empty.\n");
						printf("Please, enter new patronymic: ");
						String curPatr;
						status = GettingPatronymic(&curPatr);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							FreeEditReminders(&editReminders, countEdits);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						editReminders[countEdits].changedOption = ANSWER_PATRONYMIC;
						editReminders[countEdits].changedString.symbols = NULL;
						CopyStringsOld(&editReminders[countEdits].changedString, &ptrEdit->currentLiver.patronymic);
						editReminders[countEdits].id = id;
						countEdits++;
						printf("Patronymic is %s\n", curPatr.symbols);
						CopyStringsOld(&ptrEdit->currentLiver.patronymic, &curPatr);
						printf("Information was edit successfully.\n");
						printf("---------------------------------------------------------------------------------\n");
						DestroyString(&curPatr);
						break;
					case ANSWER_GENDER:
						printf("You choose change gender. Gender is M for man and W for woman.\n");
						printf("Please, enter gender: ");
						gender current;
						status = GettingGender(&current);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							FreeEditReminders(&editReminders, countEdits);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						editReminders[countEdits].changedOption = ANSWER_GENDER;
						editReminders[countEdits].changedGender = ptrEdit->currentLiver.genderLiver;
						editReminders[countEdits].id = id;
						countEdits++;
						if (current == GENDER_MAN) {
							printf("Gender is man.\n");
						} else
							printf("Gender is woman.\n");
						ptrEdit->currentLiver.genderLiver = current;
						printf("Information was edit successfully.\n");
						printf("---------------------------------------------------------------------------------\n");
						break;
					case ANSWER_BIRTHDAY:
						printf(
						    "You choose change birthday date. It should be written like\n"
						    "dd.MM.yyyy and can't be later today\n");
						printf("Please, enter new birthday date: ");
						int day, month, year;
						status = GettingBirthday(&day, &month, &year);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							FreeEditReminders(&editReminders, countEdits);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						editReminders[countEdits].id = id;
						editReminders[countEdits].changedOption = ANSWER_BIRTHDAY;
						editReminders[countEdits].day = ptrEdit->currentLiver.dayBirthday;
						editReminders[countEdits].month = ptrEdit->currentLiver.monthBirthday;
						editReminders[countEdits].year = ptrEdit->currentLiver.yearBirthday;
						countEdits++;
						ptrEdit->currentLiver.dayBirthday = day;
						ptrEdit->currentLiver.monthBirthday = month;
						ptrEdit->currentLiver.yearBirthday = year;
						printf("Birthday date is %02d.%02d.%d\n", day, month, year);
						printf("Information was edit successfully.\n");
						printf("---------------------------------------------------------------------------------\n");
						break;
					case ANSWER_SALARY:
						printf("You choose change monthly revenue. It is non-negative real number.\n");
						printf("Please, enter new monthly revenue: ");
						double salary;
						status = GettingSalary(&salary);
						if (status == MEMORY_ALLOCATION_ERROR) {
							DeleteLiverList(&head);
							printf("Memory allocation error.\n");
							free(laterActions);
							FreeEditReminders(&editReminders, countEdits);
							free(addedID);
							FreeLiverData(&deletedLivers, countDeletedLivers);
							return 1;
						}
						editReminders[countEdits].changedOption = ANSWER_SALARY;
						editReminders[countEdits].id = id;
						editReminders[countEdits].salary = ptrEdit->currentLiver.salary;
						countEdits++;
						printf("Monthly revenue is %lf\n", salary);
						ptrEdit->currentLiver.salary = salary;
						printf("Information was edit successfully.\n");
						printf("---------------------------------------------------------------------------------\n");
						break;
				}
				break;
			case OPTION_DELETE:
				printf(
				    "You choose option 'DELETE'. You can use this option to delete information\n"
				    "about a liver. Do you want to continue? Type yes or no.\n");
				currentAnswer = GetAnswer();
				if (currentAnswer == ANSWER_NO) {
					printf("You choose no. Select another option or try again.\n");
					break;
				} else if (currentAnswer != ANSWER_YES) {
					printf("Unknown answer. Select another option or try again.\n");
					break;
				}
				printf("---------------------------------------------------------------------------------\n");
				printf(
				    "You choose yes. Now you will be asked to enter the surname\n"
				    "of the liver, after which the program will give out all livers with\n"
				    "this surname. Select the desired ID and continue.\n");
				printf("---------------------------------------------------------------------------------\n");
				printf("Please, enter the surname: ");
				status = GettingSurname(&surname);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				printf("Surname is %s\n", surname.symbols);
				res = PrintSurnameFound(surname.symbols, head);
				DestroyString(&surname);
				if (res == 0) {
					printf("There is no livers with this surname. Nothing to delete.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				printf("Please, enter the selected ID: ");
				status = ConsoleReadInteger(&id);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					free(addedID);
					return 1;
				}
				if (status == INCORRECT_DATA) {
					printf("ID contains incorrect symbols. Restart the option.\n");
					break;
				}
				printf("ID is %d. Do you want to continue? Type yes or no.\n", id);
				currentAnswer = GetAnswer();
				if (currentAnswer == ANSWER_NO) {
					printf("You choose no. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				} else if (currentAnswer != ANSWER_YES) {
					printf("Unknown answer. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				LiverNode* currentPtr = head;
				while (currentPtr->LiverID != id && currentPtr->next != NULL) {
					currentPtr = currentPtr->next;
				}
				if (currentPtr->next == NULL) {
					printf("There is no liver with this ID.\n");
					break;
				}
				if (countAllOperations + 1 > totalOperationsLength) {
					totalOperationsLength *= 2;
					options* tempOpt = (options*)realloc(laterActions, sizeof(options) * totalOperationsLength);
					if (tempOpt == NULL) {
						DeleteLiverList(&head);
						free(laterActions);
						FreeEditReminders(&editReminders, countEdits);
						FreeLiverData(&deletedLivers, countDeletedLivers);
						printf("Memory allocation error.\n");
						free(addedID);
						return 1;
					}
					laterActions = tempOpt;
				}
				laterActions[countAllOperations++] = OPTION_DELETE;
				if (countAllOperations % 2 == 0) {
					countOperationsUndo++;
				}
				if (countDeletedLivers + 1 > totalLengthDeleted) {
					totalLengthDeleted *= 2;
					Liver* temp = (Liver*)realloc(deletedLivers, sizeof(Liver) * totalLengthDeleted);
					if (temp == NULL) {
						DeleteLiverList(&head);
						free(laterActions);
						FreeEditReminders(&editReminders, countEdits);
						FreeLiverData(&deletedLivers, countDeletedLivers);
						printf("Memory allocation error.\n");
						free(addedID);
						return 1;
					}
					deletedLivers = temp;
				}
				deletedLivers[countDeletedLivers].name.symbols = NULL;
				CopyStringsOld(&deletedLivers[countDeletedLivers].name, &currentPtr->currentLiver.name);
				deletedLivers[countDeletedLivers].surname.symbols = NULL;
				CopyStringsOld(&deletedLivers[countDeletedLivers].surname, &currentPtr->currentLiver.surname);
				deletedLivers[countDeletedLivers].patronymic.symbols = NULL;
				CopyStringsOld(&deletedLivers[countDeletedLivers].patronymic, &currentPtr->currentLiver.patronymic);
				deletedLivers[countDeletedLivers].dayBirthday = currentPtr->currentLiver.dayBirthday;
				deletedLivers[countDeletedLivers].monthBirthday = currentPtr->currentLiver.monthBirthday;
				deletedLivers[countDeletedLivers].yearBirthday = currentPtr->currentLiver.yearBirthday;
				deletedLivers[countDeletedLivers].genderLiver = currentPtr->currentLiver.genderLiver;
				deletedLivers[countDeletedLivers].salary = currentPtr->currentLiver.salary;
				DeleteLiverByID(head, id);
				printf("Information deleted successfully.\n");
				printf("---------------------------------------------------------------------------------\n");
				countDeletedLivers++;
				break;
			case OPTION_ADD:
				printf(
				    "You choose option 'ADD'. You can use this option to add information about a\n"
				    "liver. Do you want to continue? Type yes or no.\n");
				currentAnswer = GetAnswer();
				if (currentAnswer == ANSWER_NO) {
					printf("You choose no. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				} else if (currentAnswer != ANSWER_YES) {
					printf("Unknown answer. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				printf("You choose yes.\n");
				printf("---------------------------------------------------------------------------------\n");
				printf(
				    "You will be asked to fill in the user information. if you make a mistake, the program\n"
				    "will wait for the correct input\n");
				printf("---------------------------------------------------------------------------------\n");
				printf("Surname can contain only latin letters in any case.\n");
				printf("Please, enter the surname: ");
				status = GettingSurname(&surname);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				printf("Surname is %s\n", surname.symbols);
				printf("---------------------------------------------------------------------------------\n");
				printf("Name can contain only latin letters in any case.\n");
				printf("Please, enter the name: ");
				String name;
				status = GettingName(&name);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DestroyString(&surname);
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				printf("Name is %s\n", name.symbols);
				printf("---------------------------------------------------------------------------------\n");
				printf("Patronymic can contain only latin letters in any case or be empty.\n");
				printf("Please, enter the patronymic: ");
				String patronymic;
				status = GettingPatronymic(&patronymic);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DestroyString(&surname);
					DestroyString(&name);
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				printf("Patronymic is %s\n", patronymic.symbols);
				printf("---------------------------------------------------------------------------------\n");
				printf("Date of birth should be written like dd.MM.yyyy and can't be later today\n");
				printf("Please, enter the date of birth: ");
				int day, month, year;
				status = GettingBirthday(&day, &month, &year);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DestroyString(&surname);
					DestroyString(&name);
					DestroyString(&patronymic);
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				printf("Birthday is %02d.%02d.%d\n", day, month, year);
				printf("---------------------------------------------------------------------------------\n");
				printf("Monthly revenue is non-negative real number.\n");
				printf("Please, enter the monthly revenue: ");
				double salary;
				status = GettingSalary(&salary);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DestroyString(&surname);
					DestroyString(&name);
					DestroyString(&patronymic);
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				printf("Monthly revenue is %lf\n", salary);
				printf("---------------------------------------------------------------------------------\n");
				printf("Man denoted by M and woman denoted by W.\n");
				printf("Please, enter the gender: ");
				gender currentGender;
				status = GettingGender(&currentGender);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DestroyString(&surname);
					DestroyString(&name);
					DestroyString(&patronymic);
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				LiverNode* newLiver =
				    CreateLiverNode(&surname, &name, &patronymic, day, month, year, salary, countLivers, currentGender);
				countLivers++;
				PushLiverNode(&head, newLiver);
				DestroyString(&surname);
				DestroyString(&name);
				DestroyString(&patronymic);
				if (countAllOperations + 1 > totalOperationsLength) {
					totalOperationsLength *= 2;
					options* tempOpt = (options*)realloc(laterActions, sizeof(options) * totalOperationsLength);
					if (tempOpt == NULL) {
						DeleteLiverList(&head);
						free(laterActions);
						FreeEditReminders(&editReminders, countEdits);
						printf("Memory allocation error.\n");
						free(addedID);
						FreeLiverData(&deletedLivers, countDeletedLivers);
						return 1;
					}
					laterActions = tempOpt;
				}
				laterActions[countAllOperations++] = OPTION_ADD;
				if (countAllOperations % 2 == 0) {
					countOperationsUndo++;
				}
				if (countAddedID + 1 > totalAddedID) {
					totalAddedID *= 2;
					int* tempAdd = (int*)realloc(addedID, sizeof(int) * totalAddedID);
					if (tempAdd == NULL) {
						DeleteLiverList(&head);
						free(laterActions);
						FreeEditReminders(&editReminders, countEdits);
						free(addedID);
						FreeLiverData(&deletedLivers, countDeletedLivers);
						printf("Memory allocation error.\n");
						return 1;
					}
					addedID = tempAdd;
				}
				addedID[countAddedID++] = countLivers - 1;
				printf("---------------------------------------------------------------------------------\n");
				printf("Information was filled successfully.\n");
				printf("---------------------------------------------------------------------------------\n");
				break;
			case OPTION_WRITE_TO_FILE:
				printf("You choose option 'WRITE TO FILE'.\n");
				printf(
				    "You can use this option to write information into file.\n"
				    "Do you want to continue? Type yes or no.\n");
				currentAnswer = GetAnswer();
				if (currentAnswer == ANSWER_NO) {
					printf("You choose no. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				} else if (currentAnswer != ANSWER_YES) {
					printf("Unknown answer. Select another option or try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				printf(
				    "Please, enter the name of file where you want to add this info."
				    "The file name can contain only latin letters, digits and _\n");
				char* fileName;
				status = GetString(&fileName);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DeleteLiverList(&head);
					printf("Memory allocation error.\n");
					free(laterActions);
					FreeEditReminders(&editReminders, countEdits);
					free(addedID);
					FreeLiverData(&deletedLivers, countDeletedLivers);
					return 1;
				}
				status = IsValidFileName(fileName);
				if (status == INCORRECT_DATA) {
					free(fileName);
					printf(
					    "The name of file is incorrect. The name of file can contain\n"
					    "digits, latin letters in any case and _\n"
					    "Try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				FILE* currentFile = fopen(fileName, "w");
				if (currentFile == NULL) {
					free(fileName);
					printf("An error while opening file. Try again.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				LiverNode* ptr = head;
				while (ptr != NULL) {
					fprintf(currentFile, "--------------------------------------------------------------\n");
					fprintf(currentFile, "Liver's ID: %d\n", ptr->LiverID);
					fprintf(currentFile, "Surname: %s\n", ptr->currentLiver.surname.symbols);
					fprintf(currentFile, "Name: %s\n", ptr->currentLiver.name.symbols);
					fprintf(currentFile, "Patronymic: %s\n", ptr->currentLiver.patronymic.symbols);
					if (ptr->currentLiver.genderLiver == GENDER_MAN)
						fprintf(currentFile, "Gender: man\n");
					else
						fprintf(currentFile, "Gender: woman\n");
					fprintf(currentFile, "Birthday: %02d:%02d:%d\n", ptr->currentLiver.dayBirthday,
					        ptr->currentLiver.monthBirthday, ptr->currentLiver.yearBirthday);
					fprintf(currentFile, "Monthly revenue: %lf\n", ptr->currentLiver.salary);
					fprintf(currentFile, "--------------------------------------------------------------\n");
					ptr = ptr->next;
				}
				printf("Information was filled successfully.\n");
				printf("---------------------------------------------------------------------------------\n");
				free(fileName);
				fclose(currentFile);
				break;
			case OPTION_CANCEL:
				printf("You choose option 'UNDO'\n");
				if (countAllOperations == 0) {
					printf("There is nothing to undo. Current amount operations is 0.\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				if (countOperationsUndo == 0) {
					printf("It is impossible to undo this options (only last half).\n");
					printf("---------------------------------------------------------------------------------\n");
					break;
				}
				if (laterActions[countAllOperations - 1] == OPTION_ADD) {
					int deleteID = addedID[countAddedID - 1];
					DeleteLiverByID(head, deleteID);
					countAddedID--;
					countAllOperations--;
					countOperationsUndo--;
					printf(
					    "The previous option was operation add. Successfully\n"
					    "deleted.");
				} else if (laterActions[countAllOperations - 1] == OPTION_EDIT) {
					LiverNode* ptrFind = head;
					while (ptrFind->LiverID != editReminders[countEdits - 1].id) {
						ptrFind = ptrFind->next;
					}
					switch (editReminders[countEdits - 1].changedOption) {
						case ANSWER_GENDER:
							ptrFind->currentLiver.genderLiver = editReminders[countEdits - 1].changedGender;
							break;
						case ANSWER_SURNAME:
							CopyStringsOld(&ptrFind->currentLiver.surname,
							               &editReminders[countEdits - 1].changedString);
							DestroyString(&editReminders[countEdits - 1].changedString);
							break;
						case ANSWER_NAME:
							CopyStringsOld(&ptrFind->currentLiver.name, &editReminders[countEdits - 1].changedString);
							DestroyString(&editReminders[countEdits - 1].changedString);
							break;
						case ANSWER_PATRONYMIC:
							CopyStringsOld(&ptrFind->currentLiver.patronymic,
							               &editReminders[countEdits - 1].changedString);
							DestroyString(&editReminders[countEdits - 1].changedString);
							break;
						case ANSWER_SALARY:
							ptrFind->currentLiver.salary = editReminders[countEdits - 1].salary;
							break;
						case ANSWER_BIRTHDAY:
							ptrFind->currentLiver.dayBirthday = editReminders[countEdits - 1].day;
							ptrFind->currentLiver.monthBirthday = editReminders[countEdits - 1].month;
							ptrFind->currentLiver.yearBirthday = editReminders[countEdits - 1].year;
							break;
					}
					countEdits--;
					countAllOperations--;
					countOperationsUndo--;
					printf("The previous option was option edit. Successfully changes were canceled.\n");
				} else if (laterActions[countAllOperations - 1] == OPTION_DELETE) {
					Liver curr = deletedLivers[countDeletedLivers - 1];
					LiverNode* deleted = CreateLiverNode(&curr.surname, &curr.name, &curr.patronymic, curr.dayBirthday,
					                                     curr.monthBirthday, curr.yearBirthday, curr.salary,
					                                     100 + countLivers, curr.genderLiver);
					PushLiverNode(&head, deleted);
					DestroyString(&deletedLivers[countDeletedLivers - 1].name);
					DestroyString(&deletedLivers[countDeletedLivers - 1].surname);
					DestroyString(&deletedLivers[countDeletedLivers - 1].patronymic);
					countDeletedLivers--;
					countAllOperations--;
					countOperationsUndo--;
					printf(
					    "The previous option was option delete. Successfully the element\n"
					    "was restored.\n");
				}
				break;
			case OPTION_NONE:
				printf("Unknown option. Try again.\n");
				GetMenu();
				printf("---------------------------------------------------------------------------------\n");
				break;
		}
	}
	printf("                                      End\n");
	printf("---------------------------------------------------------------------------------\n");
	while (head != NULL) {
		LiverNode* ptr = head;
		head = head->next;
		DestroyString(&ptr->currentLiver.surname);
		DestroyString(&ptr->currentLiver.name);
		DestroyString(&ptr->currentLiver.patronymic);
		free(ptr);
	}
	free(addedID);
	FreeLiverData(&deletedLivers, countDeletedLivers);
	FreeEditReminders(&editReminders, countEdits);
	free(laterActions);
	return 0;
}

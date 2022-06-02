#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<Windows.h>

struct library
{
char bk_name[30];
char author[30];
int pages;
float price;
};

struct IssuedBook
{
char issued_bk_name[30];
char studentName[30];
char issueDate[30];
char returnDate[30];
};

int main()
{
struct library l[100];
struct IssuedBook issuedBookStruct[100];
char ar_nm[30], bk_nm[30];
int i, j, keepcount, issuedBookCount;
i = j = keepcount = issuedBookCount = 0;

printf("\n=============Library Management=============");

while (j != 8)
{
printf("\n\n1. Add book information\n2. Display book information\n");
printf("3. List all books of given author\n");
printf("4. List the title of specified book\n");
printf("5. List the count of books in the library\n");
printf("6. Issue a book\n7. Display the details of issued book/s\n");
printf("8. Exit");

printf("\n\nEnter one of the above : ");
scanf("%d", &j);

switch (j)
{
case 1:

printf("Enter book name = ");
scanf("%s", l[keepcount].bk_name);

printf("Enter author name = ");
scanf("%s", l[keepcount].author);

printf("Enter pages = ");
scanf("%d", &l[keepcount].pages);

printf("Enter price = ");
scanf("%f", &l[keepcount].price);

++keepcount;

break;
case 2:
if (keepcount)
{
printf("======================================================================\n");
printf("\nList of available book/s\n\n");
printf("----------------------------------------------------------------------\n");
for (i = 0; i < keepcount; i++)
{
printf("book name = %s\n", l[i].bk_name);

printf("author name = %s\n", l[i].author);

printf("pages = %d\n", l[i].pages);

printf("price = %f\n", l[i].price);

printf("----------------------------------------------------------------------\n");
}
printf("======================================================================\n");
}
else
MessageBox(0, "Book is not available in the library..!", "COUNT ZERO", 1);
break;

case 3:
printf("Enter author name : ");
scanf("%s", ar_nm);
for (i = 0; i < keepcount; i++)
{
if (strcmp(ar_nm, l[i].author) == 0)
printf("%s %s %d %f", l[i].bk_name, l[i].author, l[i].pages, l[i].price);
}
break;

case 4:
printf("Enter book name : ");
scanf("%s", bk_nm);
for (i = 0; i < keepcount; i++)
{
if (strcmp(bk_nm, l[i].bk_name) == 0)
printf("%s \t %s \t %d \t %f", l[i].bk_name, l[i].author, l[i].pages, l[i].price);
}
break;

case 5:
printf("\n No of books in library : %d", keepcount);
break;

case 6:
if (keepcount == issuedBookCount)
MessageBox(0, "Book is not available in the library..!", "COUNT ZERO", 1);
else
{
int selecteOption;
printf("\n Do you want to issue a book? \n Enter 1 for yes or 2 for no ");
scanf("%d", &selecteOption);

if (selecteOption == 1)
{
printf("issued book name = ");
scanf("%s", bk_nm);

bool isBookValid = false;
for (i = 0; i < keepcount; i++)
{
if (strcmp(bk_nm, l[issuedBookCount].bk_name) == 0)
{
isBookValid = true;

strcpy(issuedBookStruct[issuedBookCount].issued_bk_name, bk_nm);

printf("student name = ");
scanf("%s", issuedBookStruct[issuedBookCount].studentName);

time_t t = time(NULL);
struct tm *date = localtime(&t);

strcpy(issuedBookStruct[issuedBookCount].issueDate, asctime(date));
date->tm_mday += 14;
strcpy(issuedBookStruct[issuedBookCount].returnDate, asctime(date));

++issuedBookCount;
}
}

if (!isBookValid)
MessageBox(0, "You have entered an invalid book name..!", "INVALID BOOK", 1);
}
}
break;

case 7:
if (issuedBookCount)
{
printf("======================================================================\n");
printf("\nList of issued book/s\n\n");
printf("----------------------------------------------------------------------\n");
for (i = 0; i < issuedBookCount; i++)
{
printf("issued book name = %s\n", issuedBookStruct[i].issued_bk_name);

printf("student name = %s\n", issuedBookStruct[i].studentName);

printf("issue date = %s\n", issuedBookStruct[i].issueDate);

printf("return date = %s\n", issuedBookStruct[i].returnDate);

printf("----------------------------------------------------------------------\n");
}
printf("======================================================================\n");
}
else
MessageBox(0, "Book is not issued yet..!", "COUNT ZERO", 1);
break;

case 8:
exit(0);

}
}
return 0;
}

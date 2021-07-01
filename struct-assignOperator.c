#include<stdio.h>
#include<stdlib.h>

void main()
{
	// create structure
	struct student{
		char name[20];
		int age;
		float marks;
	};

	// create structure object

	// (value)
	struct student noMatric;

	// (pointer)
	struct student* CB20121;

	// Assign memory for pointer
	CB20121 = (struct student*)malloc(sizeof(struct student));

	// assign value
	noMatric.age = 18; // value
	CB20121->age = 21; // pointer

	// Print assigned value
	printf("Age of noMatric : %d\n", noMatric.age); // value
	printf("Age fo CB20121  : %d\n", CB20121->age); // pointer
}

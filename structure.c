/*
 * structure.c
 *
 *  Created on: 17 mar 2016
 *      Author: piternet
 */

#include "structure.h"
#include <stdlib.h>
#include <string.h>

typedef struct Disease {
	char *description;
	int refCounter; // reference counter - counts how many patients are 'using' this disease at some moment
} Disease;

typedef struct DiseaseList {
	Disease *disease;
	struct DiseaseList *next;
} DiseaseList;

typedef struct Patient {
	char *name; 
	DiseaseList *diseases;
	DiseaseList *lastDisease; // pointer to last disease of patient, enables to add new diseases in O(1)
} Patient;

typedef struct PatientList {
	Patient *patient;
	struct PatientList *next;
} PatientList;

PatientList *patientListHead = NULL; // list of all patients
PatientList *lastPatient = NULL; // pointer to last patient
int amountOfDiseases = 0; // global variable, storing amount of unique diseases

int getAmountOfDiseases() {
	return amountOfDiseases;
}

// function adds patient with given name to the end of patients list
PatientList* addPatient(char *name) {
	if(patientListHead == NULL) { // patientList if empty
		patientListHead = (PatientList*) malloc(sizeof(PatientList));
		patientListHead->patient = (Patient*) malloc(sizeof(Patient));
		patientListHead->patient->name = (char*) malloc((strlen(name)+1)*sizeof(char));
		strcpy(patientListHead->patient->name, name);
		patientListHead->patient->diseases = NULL;
		patientListHead->patient->lastDisease = NULL;
		patientListHead->next = NULL;
		lastPatient = patientListHead;
		return patientListHead;
	}
	else {
		PatientList *current = lastPatient;
		// current->next is pointer of our new patient
		current->next = (PatientList*) malloc(sizeof(PatientList));
		current->next->patient = (Patient*) malloc(sizeof(Patient));
		current->next->patient->name = (char*) malloc((strlen(name)+1)*sizeof(char));
		strcpy(current->next->patient->name, name);
		current->next->patient->diseases = NULL;
		current->next->patient->lastDisease = NULL;
		current->next->next = NULL;
		lastPatient = current->next;
		return current->next;
	}
}

// function finds if there is patient with given name; returns NULL if not
PatientList* findPatient(char *name) {
	/* f */
	PatientList *ptr = patientListHead;
	while(ptr != NULL) {
		if(strcmp(ptr->patient->name, name) == 0)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;
}

// function creates new disease with given description and adds it to the end of patient's diseases list
void pushDisease(Patient *patient, char *description) {
	if(patient->diseases == NULL) { // list of diseases is empty
		patient->diseases = (DiseaseList*) malloc(sizeof(DiseaseList)); // makes space for list of diseases
		patient->diseases->disease = (Disease*) malloc(sizeof(Disease)); // makes space for first element on the list of diseases
		patient->diseases->disease->description = (char*) malloc((strlen(description)+1)*sizeof(char)); // makes space for description of disease, new pointer
		strcpy(patient->diseases->disease->description, description); // copies given description into the structer
		patient->diseases->disease->refCounter = 1; 
		patient->diseases->next = NULL;
		patient->lastDisease = patient->diseases;
	}
	else {
		DiseaseList *current = patient->lastDisease;
		// current->next is pointer of our new disease
		current->next = (DiseaseList*) malloc(sizeof(DiseaseList));
		current->next->disease = (Disease*) malloc(sizeof(Disease));
		current->next->disease->description = (char*) malloc((strlen(description)+1)*sizeof(char));
		strcpy(current->next->disease->description, description);
		current->next->disease->refCounter = 1;
		current->next->next = NULL;
		patient->lastDisease = current->next;
	}
}


// returns poiter to n-th disease of given patient or NULL if such disease is not found
DiseaseList* findDisease(Patient *patient, int n) { 
	DiseaseList *ptr = patient->diseases;
	int it = 1; //iterator for counting disease descriptions
	while(ptr != NULL) {
		if(it == n)
		{
			return ptr;
		}
		ptr = ptr->next;
		it++;
	}
	return NULL;
}

// function returns last disease of given patient or NULL if there are no diseases
Disease* getLastDisease(Patient *patient) {
	if(patient->diseases == NULL)
		return NULL;
	return patient->lastDisease->disease;
}

// function adds given disease to the end of patient's diseases list
void pushCopiedDisease(Patient *patient, Disease *disease) {
	if(patient->diseases == NULL) { // list of diseases is empty
		patient->diseases = (DiseaseList*) malloc(sizeof(DiseaseList)); // makes space for list of diseases
		patient->diseases->disease = disease;
		patient->diseases->disease->refCounter = patient->diseases->disease->refCounter + 1; 
		patient->diseases->next = NULL;
		patient->lastDisease = patient->diseases;
	}
	else {
		DiseaseList *current = patient->lastDisease;
		// current->next is pointer of our new disease
		current->next = (DiseaseList*) malloc(sizeof(DiseaseList));
		current->next->disease = disease;
		current->next->disease->refCounter = current->next->disease->refCounter + 1;
		current->next->next = NULL;
		patient->lastDisease = current->next;

	}
}

int addNewDiseaseDescription(char name[], char description[]) {
	PatientList *current = findPatient(name);
	if(current == NULL)
		current = addPatient(name);
	Patient *patient = current->patient; // pointer for patient with given name
	pushDisease(patient, description);
	amountOfDiseases++;
	return 0;
}

int copyDiseaseDescription(char name1[], char name2[]) {
	PatientList *currentPatientList1 = findPatient(name1), *currentPatientList2 = findPatient(name2);
	if(currentPatientList2 == NULL)
		return 1; // error code, patient with name2 doesn't exist
	if(currentPatientList1 == NULL)
		currentPatientList1 = addPatient(name1);
	Disease *disease = getLastDisease(currentPatientList2->patient);
	if(disease == NULL)
		return 1; // error code, patient with name2 doesn't have any diseases, so nothing can be copiedion);
	pushCopiedDisease(currentPatientList1->patient, disease);
	return 0;
}

int changeDiseaseDescription(char name[], int n, char description[]) {

	PatientList *currentPatientList = findPatient(name);
	if(currentPatientList == NULL) // patient doesn't exist, return error code 
		return 1;
	DiseaseList *currentDiseaseList = findDisease(currentPatientList->patient, n);
	if(currentDiseaseList == NULL) {
		return 1; // disease with given number doesn't exist
	}	
	// delete our disease if it's not used by someone else
	currentDiseaseList->disease->refCounter = currentDiseaseList->disease->refCounter - 1;
	if(currentDiseaseList->disease->refCounter == 0) {
		free(currentDiseaseList->disease->description);
		currentDiseaseList->disease->description = NULL;
		free(currentDiseaseList->disease);
		currentDiseaseList->disease = NULL;
		amountOfDiseases--;
	}
	// create new disease
	currentDiseaseList->disease = (Disease*) malloc(sizeof(Disease));
	currentDiseaseList->disease->description = (char*) malloc((strlen(description)+1)*sizeof(char));
	strcpy(currentDiseaseList->disease->description, description);
	currentDiseaseList->disease->refCounter = 1;
	amountOfDiseases++;
	return 0;
}

const char* getPatientDescription(char name[], int n) {
	PatientList *currentPatientList = findPatient(name);
	if(currentPatientList == NULL)
		return NULL;
	Patient *patient = currentPatientList->patient;
	DiseaseList *diseaseList = findDisease(patient, n);
	if(diseaseList == NULL)
		return NULL;
	return diseaseList->disease->description;
}

int deletePatient(char name[]) {
	PatientList *currentPatientList = findPatient(name);
	if(currentPatientList == NULL) // patient doesn't exist, return error code 
		return 1;
	DiseaseList *ptr = currentPatientList->patient->diseases;
	while(ptr != NULL) {
		DiseaseList *next = ptr->next;
		Disease *disease = ptr->disease;
		disease->refCounter = disease->refCounter - 1; 
		// disease and description can only be freed when no one else uses it, i.e. refCounter == 0
		if(disease->refCounter == 0) {
			free(disease->description);
			free(disease);
			amountOfDiseases--;
		}
		free(ptr);
		ptr = next;
		
	}
	currentPatientList->patient->diseases = NULL;
	return 0;
}

// function frees free every allocated memory
void freeMemory() {
	PatientList *currentPatientList = patientListHead, *nextPatientList = NULL;
	while(currentPatientList != NULL) {
		nextPatientList = currentPatientList->next;
		DiseaseList *currentDiseaseList = currentPatientList->patient->diseases, *nextDiseaseList = NULL;
		while(currentDiseaseList != NULL) {
			nextDiseaseList = currentDiseaseList->next;
			currentDiseaseList->disease->refCounter = currentDiseaseList->disease->refCounter - 1;
			// disease and description can only be freed when no one else uses it, i.e. refCounter == 0
			if(currentDiseaseList->disease->refCounter == 0) {
				free(currentDiseaseList->disease->description);
				free(currentDiseaseList->disease);
			}
			currentDiseaseList->disease = NULL;
			free(currentDiseaseList);
			currentDiseaseList = nextDiseaseList;
		}
		free(currentPatientList->patient->name);
		free(currentPatientList->patient);
		free(currentPatientList);
		currentPatientList = nextPatientList;
	}
}
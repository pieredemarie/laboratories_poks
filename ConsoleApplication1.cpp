#include <stdio.h>
#include <fstream>
#include <locale.h>
#include <windows.h>
#include <string.h>
FILE* nalog_rate;
FILE* nalogi;
struct taxes {
	float c1, c2, c3, c4, c5;
} tax;
struct Section {
	char owner[255];
	char adress[255];
	int nomer, zone, ploshad;
} sect;
struct Tree {
	Section data;
	Tree* left, *right;
};
void Add_tree(Tree** root, Section sect) {
	if (!*root) {
		*root = new Tree;
		strcpy_s((*root)->data.owner, sect.owner);
		strcpy_s((*root)->data.adress, sect.adress);
		(*root)->data.nomer = sect.nomer;
		(*root)->data.ploshad = sect.ploshad;
		(*root)->data.zone = sect.zone;
		(*root)->left = NULL;
		(*root)->right = NULL;
	}
	else {
		if (strcmp(sect.owner, (*root)->data.owner) < 0)
			Add_tree(&(*root)->left, sect);
		else {
			Add_tree(&(*root)->right, sect);
		}
	}
}
Tree* SearchTree(Tree* root, char* searchOwner) {
	if (root == NULL)
		return NULL;
	int cmp = strcmp(searchOwner, root->data.owner);
	if (cmp == 0)
		return root;
	else if (cmp < 0)
		return SearchTree(root->left, searchOwner);
	else
		return SearchTree(root->right, searchOwner);
}
void ViewTree(Tree* root) {
	if (root == NULL) {
		return;
	}
	printf("Владелец: %s\n Адрес: %s\n Номер: %d\n Зона: %d\n Площадь: %d\n\n",
		root->data.owner, root->data.adress, root->data.nomer, root->data.zone, root->data.ploshad);
	ViewTree(root->right);
	ViewTree(root->left);
}
void CreateTree(Tree** root) {
	fopen_s(&nalogi, "nalogi.dat", "rb");
	while (fread(&sect, sizeof(Section), 1, nalogi)) {
		Add_tree(root, sect);
	}
	fclose(nalogi);
}
void SearchAndViewTree() {
	Tree* root = NULL;
	CreateTree(&root);
	ViewTree(root);
	printf("Найти определенного владельца? (Д/Н):");
	rewind(stdin);
	char option = getchar();
	rewind(stdin);
	if (option == 'д' || option == 'Д') {
		do {
			printf("Введите фамилию владельца: ");
			char searchfam[255];
			rewind(stdin);
			scanf_s("%s", searchfam, sizeof(searchfam));
			rewind(stdin);
			printf("Осуществляем поиск...\n----------------\n");
			Sleep(150);
			Tree* foundroot = SearchTree(root, searchfam);
			if (foundroot == NULL) {
				printf("Такой владелец не найден!\n");
			}
			else {
				printf("Владелец: %s\n Адрес: %s\n Номер: %d\n Зона: %d\n Площадь: %d\n\n",
					foundroot->data.owner, foundroot->data.adress, foundroot->data.nomer, foundroot->data.zone, foundroot->data.ploshad);
			}
			printf("\nПродолжить поиск? (Д/Н): ");
			rewind(stdin);
			option = getchar();
		} while ((option == 'д' || option == 'Д'));
	}
	system("cls");
}
void ShellSort(Section* arr, int n) {
	for (int d = n / 2; d >= 1; d /= 2) {
		for (int i = d; i < n; i++) {
			for (int j = i - d; j >= 0; j = j - d) {
				if ((arr[j].zone > arr[j + d].zone) || (arr[j].zone == arr[j+d].zone && strcmp(arr[j].owner,arr[j+d].owner) > 0)) {
					Section temp = arr[j];
					arr[j] = arr[j + d];
					arr[j + d] = temp;
				}
			}
		}
	}
}
void selectionSort(Section* sectArr, float* tax_arr, int* index_arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		int max_index = i;
		for (int j = i + 1; j < n; j++) {
			if (tax_arr[index_arr[j]] > tax_arr[index_arr[max_index]]) {
				max_index = j;
			}
		}
		int temp = index_arr[max_index];
		index_arr[max_index] = index_arr[i];
		index_arr[i] = temp;
	}
}
void CountingNalogi() {
	fopen_s(&nalog_rate, "nalog_rate.dat", "rb");
	fopen_s(&nalogi, "nalogi.dat", "rb");
	if (nalogi == NULL || nalog_rate == NULL)
	{
		printf("Файл не найден!\n");
		system("pause");
	}
	else {
		int count = 0;
		int ploshad_sum = 0;
		float totaltax = 0;
		float zonetax[5] = { 0 };
		int zonecount[5] = { 0,0,0,0,0 };
		int zoneArea[5] = { 0 };
		fread(&tax, sizeof(taxes), 1, nalog_rate);
		int sections_size = 0;
		while (fread(&sect, sizeof(sect), 1, nalogi)) {
			sections_size++;
		}
		rewind(nalogi);
		Section* sectArr = new Section[sections_size];
		float* tax_arr = new float[sections_size];
		int i = 0;
		printf("Фамилия        Адрес            Номер     Зона      Площадь     Налог\n");
		printf("---------------------------------------------------------------------\n");
		struct Section sect;
		while (fread(&sect, sizeof(Section), 1, nalogi))
		{
			float current_tax = 0.0;
			switch (sect.zone) {
			case 1: {
				current_tax = sect.ploshad * 10000 * tax.c1;
				break;
			}
			case 2: {
				current_tax = sect.ploshad * 10000 * tax.c1;
				break;
			}
			case 3: {
				current_tax = sect.ploshad * 10000 * tax.c1;
				break;
			}
			case 4: {
				current_tax = sect.ploshad * 10000 * tax.c1;
				break;
			}
			case 5: {
				current_tax = sect.ploshad * 10000 * tax.c1;
				break;
			}
			}
			zonetax[sect.zone - 1] += current_tax;
			zonecount[sect.zone - 1]++;
			zoneArea[sect.zone - 1] += sect.ploshad;
			totaltax += current_tax;
			ploshad_sum += sect.ploshad;
			count += 1;
			strcpy_s(sectArr[i].owner, sect.owner);
			strcpy_s(sectArr[i].adress, sect.adress);
			sectArr[i].nomer = sect.nomer;
			sectArr[i].zone = sect.zone;
			sectArr[i].ploshad = sect.ploshad;
			tax_arr[i] = current_tax;
			i++;
		}
		int* index_arr = new int[sections_size];
		for (int i = 0; i < sections_size; i++) {
			index_arr[i] = i;
		}
		selectionSort(sectArr, tax_arr, index_arr, sections_size);
		for (int i = 0; i < sections_size; i++) {
			printf("%-15s%-15s%5d%9d%12d%12.2fруб.\n",
				sectArr[index_arr[i]].owner,
				sectArr[index_arr[i]].adress,
				sectArr[index_arr[i]].nomer,
				sectArr[index_arr[i]].zone,
				sectArr[index_arr[i]].ploshad,
				tax_arr[index_arr[i]]);
		}
		printf("\n\n");
		printf("Количество участков в каждой зоне:\n");
		printf("1 зона   2 зона   3 зона   4 зона   5 зона\n");
		printf("------------------------------------------\n");
		/*for (int i = 0; i < 5; i++) {
			printf("%d) %d\n", i + 1, zonecount[i]);
		}*/
		printf("%4d%9d%9d%9d%9d", zonecount[0], zonecount[1], zonecount[2], zonecount[3], zonecount[4]);
		printf("\n\n");
		printf("Общая площадь в каждой зоне:\n");
		printf("1 зона   2 зона   3 зона   4 зона   5 зона\n");
		printf("------------------------------------------\n");
		/*for (int i = 0; i < 5; i++) {
			printf("%d) %d\n", i + 1, zoneArea[i]);
		}
		*/
		printf("%4d%9d%9d%9d%9d", zoneArea[0], zoneArea[1], zoneArea[2], zoneArea[3], zoneArea[4]);
		printf("\n\n");
		printf("Общий налог на каждую зону:\n");
		printf("  1 зона     2 зона     3 зона      4 зона     5 зона\n");
		printf("-----------------------------------------------------\n");
		/*for (int i = 0; i < 5; i++) {
			printf("%d) %.2f\n", i + 1, zonetax[i]);
		}
		*/
		printf("%10.2f%11.2f%11.2f%11.2f%11.2f\n\n", zonetax[0], zonetax[1], zonetax[2], zonetax[3], zonetax[4]);

		printf("Общая сумма налога: %f\n", totaltax);
		printf("Общая площадь всех участков: %d\n", ploshad_sum);
		printf("Общее количество участков: %d\n", count);
		delete[] sectArr;
		delete[] tax_arr;
	}
}
void printNalogi() {
	fopen_s(&nalog_rate, "nalog_rate.dat", "rb");
	if (nalog_rate == NULL)
	{
		printf("Файл не найден!\n");
	}
	else
	{
		fread(&tax, sizeof(taxes), 1, nalog_rate);
		printf("c1 = %.2f\nc2 = %.2f\nc3 = %.2f\nc4 = %.2f\nc5 = %.2f\n", tax.c1, tax.c2, tax.c3, tax.c4, tax.c5);
		fclose(nalog_rate);
	}
}
void printOwners() {
	fopen_s(&nalogi, "nalogi.dat", "rb");
	if (nalogi == NULL) {
		printf("Файл не найден!\n");
	}
	else {
		int numSections = 0;
		while (fread(&sect, sizeof(Section), 1, nalogi)) {
			numSections++;
		}
		rewind(nalogi);
		printf("Фамилия      Адрес      Номер   Зона    Площадь\n");
		printf("---------------------------------------------\n");
		int i = 0;
		Section* Allsections = new Section[numSections];
		while (fread(&Allsections[i], sizeof(Section), 1, nalogi)) {
			i++;
		};
		ShellSort(Allsections, numSections);
		for (int i = 0; i < numSections; i++) {
			printf("%-12s%-10s%5d%7d%10d\n", Allsections[i].owner, Allsections[i].adress, Allsections[i].nomer, Allsections[i].zone, Allsections[i].ploshad);
		}
		fclose(nalogi);
		delete[] Allsections;
	}
}
void EnterTaxRates() {
	fopen_s(&nalog_rate, "nalog_rate.dat", "wb");
	if (nalog_rate == NULL) {
		printf("Файл не найден!");
		system("Pause");
	}
	else {
		printf("Введите налоговые ставки: \n");
		scanf_s("%f", &tax.c1);
		scanf_s("%f", &tax.c2);
		scanf_s("%f", &tax.c3);
		scanf_s("%f", &tax.c4);
		scanf_s("%f", &tax.c5);
		fwrite(&tax, sizeof(tax), 1, nalog_rate);
		printf("\nДанные успешно введены в базу данных налоговых ставок!\n");
		fclose(nalog_rate);
	}
}
void EnterInfoTaxes() {
	fopen_s(&nalogi, "nalogi.dat", "ab");
	if (nalogi)
	{
		printf("Введите кол-во вводимых владельцев: \n");
		int n;
		scanf_s("%d", &n);
		for (int i = 0; i < n; i++) {
			struct Section sect;
			printf("Введите фамилию владельца: \n");
			scanf_s("%s", sect.owner, sizeof(sect.owner));
			printf("Введите адрес участка: \n");
			scanf_s("%s", sect.adress, sizeof(sect.adress));
			printf("Введите номер участка: \n");
			scanf_s("%d", &sect.nomer);
			printf("Введите зону участка: \n");
			scanf_s("%d", &sect.zone);
			printf("Введите площадь участка (в гектарах): \n");
			scanf_s("%d", &sect.ploshad);
			fwrite(&sect, sizeof(sect), 1, nalogi);
		}
		printf("Данные успешно записаны в базу данных");
		fclose(nalogi);
	}
	else
	{
		printf("Файл не найден!\n");
		system("PAUSE");
	}
}
int main() {
	setlocale(LC_ALL, "rus");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	int choice;
	do {
		printf("-------------------------------------\n");
		printf("БАЗА ДАННЫХ НАЛОГОВОЙ СИСТЕМЫ\n");
		printf("1 - Просмотреть текущие налоговые ставки\n");
		printf("2 - Просмотреть всю информацию об участках\n");
		printf("3 - Ввод налоговых ставок\n");
		printf("4 - Ввод информации о каждом участке\n");
		printf("5 - Расчитать налог для каждого отдельного участка\n");
		printf("6 - Просмотреть дерево владельцев участков\n");
		printf("0 - Выход из базы данных\n");
		fflush(stdin);
		scanf_s("%d", &choice);
		switch (choice) {
		case 1:
		{
			system("cls");
			printNalogi();
			break;
		}
		case 2:
		{
			system("cls");
			printOwners();
			break;
		}
		case 3:
		{
			system("cls");
			EnterTaxRates();
			break;
		}
		case 4:
		{
			system("cls");
			EnterInfoTaxes();
			break;
		}
		case 5:
		{
			system("cls");
			CountingNalogi();
			break;
		}
		case 6: {
			system("cls");
			SearchAndViewTree();
			break;
		}
		}
	} while (choice != 0);
	system("cls");
}

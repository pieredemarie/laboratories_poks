#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <fstream>
#include <string.h>
#include <vector>
FILE* stats;

struct Harvest {
	char district[255];
	int total_area, area_wheat, area_rozh, area_oves, area_yachmen, harv_wheat, harv_rozh, harv_oves, harv_yachmen;
} crop;
struct Node {
	struct Harvest data;
	struct Node* left;
	struct Node* right;
};

struct Node* createNode(struct Harvest data) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}
int getYield(struct Harvest data, int choice) {
	switch (choice) {
	case 1:
		return data.area_wheat > 0 ? data.harv_wheat / data.area_wheat : 0;
	case 2:
		return data.area_rozh > 0 ? data.harv_rozh / data.area_rozh : 0;
	case 3:
		return data.area_oves > 0 ? data.harv_oves / data.area_oves : 0;
	case 4:
		return data.area_yachmen > 0 ? data.harv_yachmen / data.area_yachmen : 0;
	default:
		return 0;
	}
}
struct Node* insertNode(struct Node* root, struct Harvest data, int choice) {
	if (root == NULL) {
		return createNode(data);
	}

	int newYield = getYield(data, choice);
	int rootYield = getYield(root->data, choice);

	if (newYield > rootYield) {
		root->right = insertNode(root->right, data, choice);
	}
	else if (newYield < rootYield) {
		root->left = insertNode(root->left, data, choice);
	}
	else {
		if (newYield == rootYield) {
			if (root->left == NULL) {
				root->left = createNode(data);
			}
			else {
				root->right = insertNode(root->right, data, choice);
			}
		}
	}

	return root;
}
void suffixTraversal(struct Node* root, int choice) {
	if (root == NULL) {
		return;
	}

	suffixTraversal(root->right, choice);

	int currentYield = getYield(root->data, choice);
	printf("District: %s\n", root->data.district);
	printf("Total Area: %d\n", root->data.total_area);
	printf("Wheat Area: %d, Yield: %d\n", root->data.area_wheat, root->data.harv_wheat);
	printf("Rye Area: %d, Yield: %d\n", root->data.area_rozh, root->data.harv_rozh);
	printf("Oats Area: %d, Yield: %d\n", root->data.area_oves, root->data.harv_oves);
	printf("Barley Area: %d, Yield: %d\n", root->data.area_yachmen, root->data.harv_yachmen);
	printf("Yield for selected grain type: %d\n\n", currentYield);

	suffixTraversal(root->left, choice);

	/*printf("Область: %s\n", root->data.district);
	printf("Общая площадь: %d\n", root->data.total_area);
	printf("Площадь пшеницы: %d, Урожай: %d\n", root->data.area_wheat, root->data.harv_wheat);
	printf("Площадь ржи: %d, Урожай: %d\n", root->data.area_rozh, root->data.harv_rozh);
	printf("Площадь овса: %d, Урожай: %d\n", root->data.area_oves, root->data.harv_oves);
	printf("Площадь ячменя: %d, Урожай: %d\n\n", root->data.area_yachmen, root->data.harv_yachmen);*/
}
int strSize(char word[]) {  // вычеслим длину строки (в виде массива символов)
	int strS = 0;
	while (word[strS] != NULL) {
		strS++;
	}
	return strS;
}
int wordsCompare(char word1[], char word2[]) {  // сравнение 2 строк 
	//int wordSize1 = strSize(word1), wordSize2 = strSize(word2), minWordSize[2];
	int wordSize1 = strlen(word1), wordSize2 = strlen(word2), minWordSize[2];
	if (wordSize1 <= wordSize2) {  // находим мин длину строки и индекс этой строки
		minWordSize[0] = wordSize1;
		minWordSize[1] = 1;
	}
	else {
		minWordSize[0] = wordSize2;
		minWordSize[1] = 0;
	}
	for (int i = 0; i < minWordSize[0]; i++) {  // сравниваем символы строк 
		if (word1[i] < word2[i]) {
			return 1;
		}
		else if (word2[i] < word1[i]) {
			return 0;
		}
	}
	return minWordSize[1]; // символы равны - возвращаем индекс той у которой меньше символов
}
void mergeSort(int m[], int left, int right, std::vector<Harvest>& H) {  // принимает массив для сортировка m,
	//  индекс начала подмассива left и конец подмассива right, H ссылка на вектор с информацией о районах
	if (left != right) {  // если равны то подмассив имеет 1 элемент и он уже отсортирован
		int middle = (left + right) / 2;  //  середина подмассива
		mergeSort(m, left, middle, H);  // рекурсивно вызываем функцию сортировки для левой части массива
		mergeSort(m, middle + 1, right, H);  // для правой
		int i = left;// Начало первого пути (левого подмассива)
		int j = middle + 1;// Начало второго пути (правого)
		int* temp = (int*)malloc(right * sizeof(int)); // Дополнительный массив с выделенной памятью для слияния
		for (int step = 0; step < right - left + 1; step++) {// Для всех элементов дополнительного массива
			// записываем в формируемую последовательность меньший из элементов двух путей
			// или остаток первого пути если j > i
			if ((j > right) || ((i <= middle) && (wordsCompare(H[m[i]].district, H[m[j]].district)))) {  // закончился ли правый подмассив
				//  закончился ли левый и сравнивает районы с помощью функции сравнения районов
				temp[step] = m[i];  // закидываем элемент левого подмассива в допмассив на позицию step
				i++;  // след элемент
			}
			else {
				temp[step] = m[j];  // закидываем элемент правого подмассива в допмассив на позицию step
				j++;  // след элемент
			}
		}
		for (int step = 0; step < right - left + 1; step++) {  // копируем отсортированные элементы из временного массива temp
			//  обратно в исходный массив m
			m[left + step] = temp[step];
		}
	}
}
void binSort(int m[], int n, std::vector<Harvest>& H) {
	for (int i = 1; i < n; i++) {
		int x = m[i], left = 0, right = i;
		while (left != right) { //пока есть что просматривать
			int middle = (left + right) / 2;
			if (H[x].total_area < H[m[middle]].total_area) {  // сравниваем общая площадь района с индексом x 
				// с общей площадью райна с индексом middle
				left = middle + 1; //ищем в правой части
			}
			else {
				right = middle; //ищем в левой части
			}
		}
		for (int j = i; j > right; j--) {  // сдвигаем элементы в массиве m вправо начиная с i до right +1
			// и вставляем туда элемент x (в m[right])
			m[j] = m[j - 1];
		}
		m[right] = x;
	}
}
void InputInfoHarvest() { // функция ввода информации о районе
	fopen_s(&stats, "stats.dat", "ab");
	if (stats == NULL)
		printf("Файл не найден\n");
	else {
		int n;
		printf("Введите количество районов: \n");
		scanf_s("%d", &n);
		for (int i = 0; i < n; i++) { // вводим все данные по району
			printf("Введите название района: \n");
			scanf_s("%s", crop.district, sizeof(crop.district));
			printf("Введите общую площадь пахотных земель: \n");
			scanf_s("%d", &crop.total_area);
			printf("Введите площадь, занятую пшеницей: \n");
			scanf_s("%d", &crop.area_wheat);
			printf("Введите площадь, занятую рожью: \n");
			scanf_s("%d", &crop.area_rozh);
			printf("Введите площадь, занятую овесом: \n");
			scanf_s("%d", &crop.area_oves);
			printf("Введите площадь, занятую ячменем: \n");
			scanf_s("%d", &crop.area_yachmen);
			printf("Введите урожай пщеницы: \n");
			scanf_s("%d", &crop.harv_wheat);
			printf("Введите урожай ржи: \n");
			scanf_s("%d", &crop.harv_rozh);
			printf("Введите урожай овса: \n");
			scanf_s("%d", &crop.harv_oves);
			printf("Введите урожай ячмень: \n");
			scanf_s("%d", &crop.harv_yachmen);
			fwrite(&crop, sizeof(crop), 1, stats); // записываем все даннные в файл
		}
		printf("Данные успешно записаны в файл!\n");
		fclose(stats);
	}
}
void OutputInfoHarvest() { // функция вывода информации о районе
	fopen_s(&stats, "stats.dat", "rb");
	if (stats == NULL)
		printf("Файл не найден\n");
	else {
		printf("Район        Общая площадь    Площадь пшеницы  Площадь рожи   Площадь овса  Площадь ячменя Урожай пшеницы  Урожай ржи  Урожай овса  Урожай ячменя\n");
		printf("--------------------------------------------------------------------------------------------------------------------------------------------------\n");
		while (fread(&crop, sizeof(crop), 1, stats)) {
			printf("%-10s%12d%15d%15d%15d%15d%15d%15d%15d%15d\n", crop.district, crop.total_area, crop.area_wheat, crop.area_rozh, crop.area_oves, crop.area_yachmen, crop.harv_wheat, crop.harv_rozh, crop.harv_oves, crop.harv_yachmen);
		}
	}
}
void mergeMenu(int n, std::vector<Harvest>& H) {  // кол-во районов, H ссылка на вектор с информацией о районах
	int m[100];
	for (int i = 0; i < n; i++) { // запонляем вектор индексами районов
		m[i] = i;
	}
	mergeSort(m, 0, n - 1, H);
	printf("Сведения по районам:\n\n");
	for (int i = 0; i < n; i++) {
		printf("%s район\nОбщая площадь пахотных земель: %dГа\nПшеница (урожай): %dГа (%dТ)\nРожь (урожай): %dГа (%dТ)\nОвес (урожай): %dГа (%dТ)\nЯчмень (урожай): %dГа (%dТ)\n\n", H[m[i]].district, H[m[i]].total_area, H[m[i]].area_wheat, H[m[i]].harv_wheat, H[m[i]].area_rozh, H[m[i]].harv_rozh, H[m[i]].area_oves, H[m[i]].harv_oves, H[m[i]].area_yachmen, H[m[i]].harv_yachmen);
	}
}
void binMenu(int n, std::vector<Harvest>& H) {
	int m[100];
	for (int i = 0; i < n; i++) {
		m[i] = i;
	}
	binSort(m, n, H);
	printf("Сведения по районам:\n\n");
	for (int i = 0; i < n; i++) {
		printf("%s район\nОбщая площадь пахотных земель: %dГа\nПшеница (урожай): %dГа (%dТ)\nРожь (урожай): %dГа (%dТ)\nОвес (урожай): %dГа (%dТ)\nЯчмень (урожай): %dГа (%dТ)\n\n", H[m[i]].district, H[m[i]].total_area, H[m[i]].area_wheat, H[m[i]].harv_wheat, H[m[i]].area_rozh, H[m[i]].harv_rozh, H[m[i]].area_oves, H[m[i]].harv_oves, H[m[i]].area_yachmen, H[m[i]].harv_yachmen);
	}
}
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");

	std::vector<Harvest> harvest; //создали вектор (это динамический массив)
	harvest.resize(100); //выделение памяти для вектора (не обязательно, но я сделал для перестраховки)

	printf("1 - Создать файл\n2-Открыть уже существующий\n");
	int controlpunkt;
	scanf_s("%d", &controlpunkt);
	if (controlpunkt == 1)
		fopen_s(&stats, "stats.dat", "wb"); // создаем новый файл или очищаем данные в уже существующем
	if (controlpunkt == 2)
		fopen_s(&stats, "stats.dat", "rb"); // открываем файл для чтения
	fclose(stats); // закрываем файл 
	system("cls"); // очищаем консоль
	printf("-----------------------------------------------------\n");
	printf("Статистическая служба\n");
	printf("1 - Ввод данных об урожае\n");
	printf("2 - Вывод информации об урожае\n");
	printf("3 - Подсчет всей необходимой информации (по заданию)\n");
	printf("4 - Сортировка слиянием\n");
	printf("5 - Сортировка вставками\n");
	printf("6 - Просмотреть дерево всей информации]n");
	printf("0 - Выход из программы\n");
	printf("-----------------------------------------------------\n");
	printf("Введите действие: \n");
	int punkt;
	do {
		scanf_s("%d", &punkt);
		switch (punkt) {
		case 1: {
			InputInfoHarvest(); // функция ввода данных
			break;
		}
		case 2: {
			OutputInfoHarvest();// функция вывода данных
			break;
		}
		case 3: {
			fopen_s(&stats, "stats.dat", "rb");
			if (stats != NULL) { // если файл открылся
				int area_oblast = 0; // площадь посевов в области (все районы суммируются)
				int harvest_rozh_obl = 0, harvest_wheat_obl = 0, harvest_oves_obl = 0, harvest_yachmen_obl = 0; // урожай пщеницы/ржи и тд в области
				float av_rozh = 0.0, av_wheat = 0.0, av_oves = 0.0, av_yachmen = 0.0; // средняя урожайность в районе
				while (fread(&crop, sizeof(crop), 1, stats)) {
					area_oblast += crop.total_area; // суммируем площадь из всех районов
					av_rozh = crop.harv_rozh / float(crop.area_rozh); // средняя урожайность ржи
					av_wheat = crop.harv_wheat / float(crop.area_wheat); // средняя урожайность пщеницы
					av_oves = crop.harv_oves / float(crop.area_oves); // средняя урожайность овса
					av_yachmen = crop.harv_yachmen / float(crop.area_yachmen); // средняя урожайность ячменя
					harvest_wheat_obl += crop.harv_wheat; // урожай пщеницы в области
					harvest_rozh_obl += crop.harv_rozh; // урожай ржи в области
					harvest_oves_obl += crop.harv_oves; // урожай овса в области
					harvest_yachmen_obl += crop.harv_yachmen; // урожай ячменя в области
					printf("В районе %s:\nСредняя урожайность пщеницы = %f\nСредняя урожайность ржи = %f\nСредняя урожайность овса = %f\nСредняя урожайность ячменя = %f\n", crop.district, av_wheat, av_rozh, av_oves, av_yachmen);
				}
				printf("В области: \n");
				printf("Урожай пщеницы = %d\nУрожай ржи = %d\nУрожай овса = %d\nУрожай ячменя = %d\n", harvest_wheat_obl, harvest_rozh_obl, harvest_oves_obl, harvest_yachmen_obl);
				printf("Площадь пахотных земель в области = %d\n", area_oblast);
				printf("Средняя урожайность пщеницы = %f\nСредняя урожайность ржи = %f\nСредняя урожайность овса = %f\nСредняя урожайность ячменя = %f\n", harvest_wheat_obl / float(area_oblast), harvest_rozh_obl / float(area_oblast), harvest_oves_obl / float(area_oblast), harvest_yachmen_obl / float(area_oblast));
			}
			else {
				printf("Файл не найден!\n");
			}
			break;
		}
		case 4:
		{
			int i = 0;
			fopen_s(&stats, "stats.dat", "rb");
			if (stats != NULL)
			{
				while (fread(&crop, sizeof(crop), 1, stats))
				{
					harvest[i] = crop;
					i++;
				}
			}
			mergeMenu(i + 1, harvest);
			break;
		}
		case 5:
		{
			int i = 0;
			fopen_s(&stats, "stats.dat", "rb");
			if (stats != NULL)
			{
				while (fread(&crop, sizeof(crop), 1, stats))
				{
					harvest[i] = crop;
					i++;
				}
			}
			binMenu(i + 1, harvest);
			break;
		}
		case 6: {
			fopen_s(&stats, "stats.dat", "rb");
			Node* root = NULL;
			int choice;
			printf("Введите тип зерна для сортировки (1: Пшеница, 2: Рожь, 3: Овес, 4: Ячмень): ");
			scanf_s("%d", &choice);
			while (fread(&crop, sizeof(struct Harvest), 1, stats)) {
				root = insertNode(root, crop, choice);
			}
			printf("Бинарное дерево информации по посеву: \n\n");
			suffixTraversal(root,choice);
			fclose(stats);
		}

		}
	} while (punkt != 0);
}
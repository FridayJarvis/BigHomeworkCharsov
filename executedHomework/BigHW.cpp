#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


class Students {
private:
	int id;

	struct FullName {
		std::string firstName;
		std::string middleName;
		std::string lastName;
	};


public:
	FullName fullName;
	std::string specialization;
	float avgScore;


	Students(int _id, const std::string _firstName, const std::string _middleName,
		const std::string _lastName, float _avgScore, const std::string _specialization)
		: id(_id), specialization(_specialization), avgScore(_avgScore) {
		fullName.firstName = _firstName;
		fullName.middleName = _middleName;
		fullName.lastName = _lastName;
	}


	int getId() const {
		return id;
	}

	std::string toString() {
		std::ostringstream oss;
		oss << id << " "
			<< fullName.firstName << " " << fullName.middleName << " " << fullName.lastName << " "
			<< avgScore << " "
			<< specialization;
		return oss.str();
	}

	static Students fromString(const std::string line) {
		std::istringstream iss(line);
		std::string token;
		std::string firstName, middleName, lastName, specialization;

		int id;

		float avgScore;


		std::getline(iss, token, ' ');
		id = std::stoi(token);
		std::getline(iss, firstName, ' ');
		std::getline(iss, middleName, ' ');
		std::getline(iss, lastName, ' ');
		std::getline(iss, token, ' ');
		avgScore = std::stof(token);
		std::getline(iss, specialization);

		return Students(id, firstName, middleName, lastName, avgScore, specialization);
	}
};

class StudentDB {
private:
	std::vector<Students> students;
	const std::string fileName = "studenstDB.txt";

	void loadFromFile() {
		std::ifstream file(fileName);
		std::string line;

		while (std::getline(file, line)) {
			students.push_back(Students::fromString(line));
		}
	}

	void saveToFile() {
		std::ofstream file(fileName, std::ios::out);
		for (auto student : students) {
			file << student.toString() << "\n";
		}
	}

public:
	StudentDB() {
		loadFromFile();
	}

	void addStudent(int id, const std::string firstName, const std::string middleName,
		const std::string lastName, float avgScore, const std::string specialization) {
		students.emplace_back(id, firstName, middleName, lastName, avgScore, specialization);
		saveToFile();
	}

	void displayStudents() {
		for (auto student : students) {
			std::cout << student.toString() << "\n";
		}
	}


	void searchStudent(const std::string criteria, const std::string value) {
		for (auto student : students) {
			if ((criteria == "id" && std::to_string(student.getId()) == value)
				|| (criteria == "name" && (student.fullName.firstName == value || student.fullName.middleName == value || student.fullName.lastName == value))
				|| (criteria == "specialization" && student.specialization == value)
				|| (criteria == "avg_score" && std::to_string(student.avgScore) == value)) {

				std::cout << student.toString() << "\n";
				return;
			}
		}
		std::cout << "Запись не найдена!\n";
	}

	void deleteStudentById(int id) {
		students.erase(std::remove_if(students.begin(), students.end(), [id](Students student) {
			return student.getId() == id;
			}), students.end());
		saveToFile();
	}
};

int main() {
	
	setlocale(LC_ALL, "Russian");
	StudentDB db;

	while (true) {
		int choice;
		std::cout << "1. Добавить студента\n"
			<< "2. Показать всех студентов\n"
			<< "3. Найти студента\n"
			<< "4. Удалить студента\n"
			<< "0. Выйти\n";
		std::cout << "Выберите действие: ";
		std::cin >> choice;

		switch (choice) {
		case 1: {
			int id;
			float avgScore;
			std::string firstName, middleName, lastName, specialization;

			std::cout << "Введите номер ИСУ: ";
			std::cin >> id;

			std::cout << "Введите имя: ";
			std::cin >> firstName;

			std::cout << "Введите фамилию: ";
			std::cin >> middleName;

			std::cout << "Введите отчество: ";
			std::cin >> lastName;

			std::cout << "Введите средний балл: ";
			std::cin >> avgScore;

			std::cout << "Введите специальность: ";
			std::cin >> specialization;

			db.addStudent(id, firstName, middleName, lastName, avgScore, specialization);
			break;
		}
		case 2:
			db.displayStudents();
			break;
		case 3: {
			std::string criteria;
			std::cout << "Введите критерий поиска (id, name, avg_score, specialization): ";
			std::cin >> criteria;

			std::string value;
			std::cout << "Введите значение поиска: ";
			std::cin >> value;

			db.searchStudent(criteria, value);
			break;
		}
		case 4: {
			int id;
			std::cout << "Введите ID студента для удаления: ";
			std::cin >> id;

			db.deleteStudentById(id);
			break;
		}
		case 0:
			std::cout << "Вы вышли из программы!\n";
			return 0;
		default:
			std::cout << "Неверный выбор меню! Попробуйте снова!\n";
		}
	}
	return 0;
}

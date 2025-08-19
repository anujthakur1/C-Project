#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <regex>
using namespace cv;
using namespace std;
namespace fs = std::filesystem;

bool isValidName(const string &str)
{
    return regex_match(str, regex("^[A-Za-z ]+$"));
}

bool isValidPhone(const string &str)
{
    return regex_match(str, regex("^[0-9]{10}$")); // 10 digit number
}

bool isValidEmail(const string &str)
{
    return regex_match(str, regex("^[\\w.-]+@[\\w.-]+\\.[A-Za-z]{2,}$"));
}

bool isValidDate(const string &str)
{
    if (!regex_match(str, regex("^[0-9]{2}/[0-9]{2}/[0-9]{4}$")))
        return false;
    int d = stoi(str.substr(0, 2));
    int m = stoi(str.substr(3, 2));
    int y = stoi(str.substr(6, 4));
    if (m < 1 || m > 12)
        return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
        daysInMonth[1] = 29;
    return (d >= 1 && d <= daysInMonth[m - 1]);
}

bool isValidText(const string &str)
{
    return regex_match(str, regex("^[A-Za-z0-9 ]+$")); // alphanumeric only
}

class IDCard
{
protected:
    string name, photoPath, folder;

public:
    virtual void getDetails() = 0;
    virtual void generateIDCard() = 0;
    virtual void showIDCard() = 0;

    void createFolder(const string &baseFolder)
    {
        folder = baseFolder + "/" + name;
        fs::create_directories(folder);
    }

    void capturePhoto()
    {
        VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Camera not found!" << endl;
            return;
        }
        cout << "Press 's' to capture photo..." << endl;
        Mat frame;
        while (true)
        {
            cap >> frame;
            imshow("Camera", frame);
            char key = waitKey(30);
            if (key == 's' || key == 'S')
            {
                string filePath = folder + "/photo.jpg";
                imwrite(filePath, frame);
                photoPath = filePath;
                cout << "Photo saved!" << endl;
                break;
            }
        }
        destroyAllWindows();
    }

    void deletePersonFolder(const string &cardTypeFolder)
    {
        if (!fs::exists(cardTypeFolder))
        {
            cout << "No folders found in " << cardTypeFolder << endl;
            return;
        }
        vector<string> peopleFolders;
        int idx = 1;
        for (auto &entry : fs::directory_iterator(cardTypeFolder))
        {
            if (entry.is_directory())
            {
                peopleFolders.push_back(entry.path().filename().string());
                cout << idx++ << ". " << entry.path().filename().string() << endl;
            }
        }
        if (peopleFolders.empty())
        {
            cout << "No person folders found!" << endl;
            return;
        }
        int choice;
        cout << "Select folder to delete: ";
        cin >> choice;
        cin.ignore();
        if (choice < 1 || choice > (int)peopleFolders.size())
        {
            cout << "Invalid choice!" << endl;
            return;
        }
        string folderToDelete = cardTypeFolder + "/" + peopleFolders[choice - 1];
        fs::remove_all(folderToDelete);
        cout << "Deleted folder: " << folderToDelete << endl;
    }
};

class StudentIDCard : public IDCard
{
    string roll, program, faculty, guardian, validDate, expiryDate, phone, email;

public:
    void getDetails() override
    {
        do
        {
            cout << "Enter Name: ";
            getline(cin, name);
        } while (!isValidName(name));
        do
        {
            cout << "Enter Roll Number: ";
            getline(cin, roll);
        } while (!isValidText(roll));
        do
        {
            cout << "Enter Program: ";
            getline(cin, program);
        } while (!isValidText(program));
        do
        {
            cout << "Enter Faculty: ";
            getline(cin, faculty);
        } while (!isValidText(faculty));
        do
        {
            cout << "Enter Guardian Name: ";
            getline(cin, guardian);
        } while (!isValidName(guardian));
        do
        {
            cout << "Enter Phone Number: ";
            getline(cin, phone);
        } while (!isValidPhone(phone));
        do
        {
            cout << "Enter Email: ";
            getline(cin, email);
        } while (!isValidEmail(email));
        do
        {
            cout << "Enter Valid Date (dd/mm/yyyy): ";
            getline(cin, validDate);
        } while (!isValidDate(validDate));
        do
        {
            cout << "Enter Expiry Date (dd/mm/yyyy): ";
            getline(cin, expiryDate);
        } while (!isValidDate(expiryDate));
    }

    void generateIDCard() override
    {
        Mat idCard(550, 380, CV_8UC3, Scalar(135, 206, 235)); // slightly bigger
        rectangle(idCard, Point(0, 0), Point(idCard.cols - 1, idCard.rows - 1), Scalar(255, 255, 255), 3);

        Mat photo = imread(photoPath);
        if (!photo.empty())
        {
            resize(photo, photo, Size(100, 100));
            photo.copyTo(idCard(Rect((idCard.cols - 100) / 2, 20, 100, 100)));
        }

        int left = 20, y = 140, gap = 40, thick = 2;
        putText(idCard, "Name: " + name, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Roll: " + roll, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Program: " + program, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Faculty: " + faculty, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Guardian: " + guardian, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Phone: " + phone, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Email: " + email, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Valid: " + validDate, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Expiry: " + expiryDate, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);

        // Signature bottom-right, one step lower (aligned with card type line)
        int sigX = idCard.cols - 140, sigY = idCard.rows - 30;
        line(idCard, Point(sigX, sigY), Point(sigX + 120, sigY), Scalar(0, 0, 0), 1);
        putText(idCard, "Signature", Point(sigX, sigY + 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);

        // Card type bottom-left
        putText(idCard, "Student ID Card", Point(10, idCard.rows - 10), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 2);

        string filePath = folder + "/IDcard.jpg";
        imwrite(filePath, idCard);
        cout << "ID Card generated at: " << filePath << endl;
    }

    void showIDCard() override
    {
        string filePath = folder + "/IDcard.jpg";
        Mat idCard = imread(filePath);
        if (idCard.empty())
        {
            cout << "ID Card not found!" << endl;
            return;
        }
        imshow("Student ID Card", idCard);
        waitKey(0);
        destroyAllWindows();
    }
};

class BusinessIDCard : public IDCard
{
    string position, company, email, phone, address;

public:
    void getDetails() override
    {
        do
        {
            cout << "Enter Name: ";
            getline(cin, name);
        } while (!isValidName(name));
        do
        {
            cout << "Enter Position: ";
            getline(cin, position);
        } while (!isValidText(position));
        do
        {
            cout << "Enter Company: ";
            getline(cin, company);
        } while (!isValidText(company));
        do
        {
            cout << "Enter Email: ";
            getline(cin, email);
        } while (!isValidEmail(email));
        do
        {
            cout << "Enter Phone: ";
            getline(cin, phone);
        } while (!isValidPhone(phone));
        do
        {
            cout << "Enter Address: ";
            getline(cin, address);
        } while (!isValidText(address));
    }

    void generateIDCard() override
    {
        Mat idCard(500, 350, CV_8UC3, Scalar(135, 206, 235));
        rectangle(idCard, Point(0, 0), Point(idCard.cols - 1, idCard.rows - 1), Scalar(255, 255, 255), 3);

        Mat photo = imread(photoPath);
        if (!photo.empty())
        {
            resize(photo, photo, Size(100, 100));
            photo.copyTo(idCard(Rect((idCard.cols - 100) / 2, 20, 100, 100)));
        }

        int left = 20, y = 140, gap = 40, thick = 2;
        putText(idCard, "Name: " + name, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Position: " + position, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Company: " + company, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Email: " + email, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Phone: " + phone, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Address: " + address, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);

        // Signature bottom-right
        int sigX = idCard.cols - 140;
        int sigY = idCard.rows - 30;
        line(idCard, Point(sigX, sigY), Point(sigX + 120, sigY), Scalar(0, 0, 0), 1);
        putText(idCard, "Signature", Point(sigX, sigY + 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);

        // Card type bottom-left
        putText(idCard, "Business ID Card", Point(10, idCard.rows - 10), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 2);

        string filePath = folder + "/IDcard.jpg";
        imwrite(filePath, idCard);
        cout << "ID Card generated at: " << filePath << endl;
    }

    void showIDCard() override
    {
        string filePath = folder + "/IDcard.jpg";
        Mat idCard = imread(filePath);
        if (idCard.empty())
        {
            cout << "ID Card not found!" << endl;
            return;
        }
        imshow("Business ID Card", idCard);
        waitKey(0);
        destroyAllWindows();
    }
};

class LibraryIDCard : public IDCard
{
    string memberID, contact, email;

public:
    void getDetails() override
    {
        do
        {
            cout << "Enter Name: ";
            getline(cin, name);
        } while (!isValidName(name));
        do
        {
            cout << "Enter Member ID: ";
            getline(cin, memberID);
        } while (!isValidText(memberID));
        do
        {
            cout << "Enter Contact: ";
            getline(cin, contact);
        } while (!isValidPhone(contact));
        do
        {
            cout << "Enter Email: ";
            getline(cin, email);
        } while (!isValidEmail(email));
    }

    void generateIDCard() override
    {
        Mat idCard(500, 350, CV_8UC3, Scalar(135, 206, 235));
        rectangle(idCard, Point(0, 0), Point(idCard.cols - 1, idCard.rows - 1), Scalar(255, 255, 255), 3);

        Mat photo = imread(photoPath);
        if (!photo.empty())
        {
            resize(photo, photo, Size(100, 100));
            photo.copyTo(idCard(Rect((idCard.cols - 100) / 2, 20, 100, 100)));
        }

        int left = 20, y = 140, gap = 40, thick = 2;
        putText(idCard, "Name: " + name, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Member ID: " + memberID, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Contact: " + contact, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);
        putText(idCard, "Email: " + email, Point(left, y += gap), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), thick);

        // Signature bottom-right
        int sigX = idCard.cols - 140;
        int sigY = idCard.rows - 30;
        line(idCard, Point(sigX, sigY), Point(sigX + 120, sigY), Scalar(0, 0, 0), 1);
        putText(idCard, "Signature", Point(sigX, sigY + 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);

        // Card type bottom-left
        putText(idCard, "Library ID Card", Point(10, idCard.rows - 10), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 2);

        string filePath = folder + "/IDcard.jpg";
        imwrite(filePath, idCard);
        cout << "ID Card generated at: " << filePath << endl;
    }

    void showIDCard() override
    {
        string filePath = folder + "/IDcard.jpg";
        Mat idCard = imread(filePath);
        if (idCard.empty())
        {
            cout << "ID Card not found!" << endl;
            return;
        }
        imshow("Library ID Card", idCard);
        waitKey(0);
        destroyAllWindows();
    }
};

int main()
{
    while (true)
    {
        cout << "\nSelect Card Type:\n1. Student ID Card\n2. Business ID Card\n3. Library ID Card\n4. Exit\nChoice: ";
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice == 4)
            break;

        IDCard *card = nullptr;
        string baseFolder;
        if (choice == 1)
        {
            card = new StudentIDCard();
            baseFolder = "StudentIDCard";
        }
        else if (choice == 2)
        {
            card = new BusinessIDCard();
            baseFolder = "BusinessIDCard";
        }
        else if (choice == 3)
        {
            card = new LibraryIDCard();
            baseFolder = "LibraryIDCard";
        }
        else
        {
            cout << "Invalid choice!\n";
            continue;
        }

        card->getDetails();
        card->createFolder(baseFolder);

        while (true)
        {
            cout << "\n1. Capture Photo\n2. Generate ID Card\n3. Show ID Card\n4. Delete Person Folder\n5. Back to Card Type Menu\nChoice: ";
            int menuChoice;
            cin >> menuChoice;
            cin.ignore();
            if (menuChoice == 1)
                card->capturePhoto();
            else if (menuChoice == 2)
                card->generateIDCard();
            else if (menuChoice == 3)
                card->showIDCard();
            else if (menuChoice == 4)
                card->deletePersonFolder(baseFolder);
            else if (menuChoice == 5)
                break;
            else
                cout << "Invalid choice!\n";
        }
        delete card;
    }
    return 0;
}

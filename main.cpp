#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cctype>
#include <random>
#include <vector>
#include <string>
#include <csignal> // Include signal handling header
#include "console_color_mod.h"
using namespace std;

using u32 = uint_least32_t;
using engine = std::mt19937;
unsigned int sleep_time = 0;
static bool interrupted = false; // Flag to indicate if the program was interrupted

std::string getRandomAndRemove(std::vector<std::string>& vec)
{
    if (vec.empty()) return "Vector is empty";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, vec.size() - 1);
    int randomIndex = dis(gen);

    std::string result = vec[randomIndex];
    std::swap(vec[randomIndex], vec.back());
    vec.pop_back();

    return result;
}

int processProfile(string profile)
{
    string request = "/Data/instaloader/instaloader.py " + profile + " +settings.txt --stories --fast-update";
    return system(request.c_str());
}

void signalHandler(int signum)
{
    interrupted = true; // Set the flag to indicate interruption
}

int main(int argc, const char* argv[])
{
    std::random_device os_seed;
    const u32 seed = os_seed();
    int total_profiles = 0;

    Color::Modifier red(Color::FG_RED);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier yellow(Color::FG_YELLOW);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier magenta(Color::FG_MAGENTA);
    Color::Modifier cyan(Color::FG_CYAN);
    Color::Modifier white(Color::FG_WHITE);
    Color::Modifier black(Color::FG_BLACK);
    Color::Modifier def(Color::FG_DEFAULT);

    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(3, 15);
    string profile = "";
    string request = "";
    int counter = 1;
    int success = 0;
    int failed = 0;
    int skipped = 0;
    // Register the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, signalHandler);

    ifstream file;
    string profile_list;
    int start = 1;
    if (argc > 1)
    {
        profile_list = argv[1];
    }
    else
    {
        cout << "Usage: ./InstagramTasker [profile_file] [start_position]" << endl;
    }
    if (argc > 2)
    {
        start = atoi(argv[2]);
    }

    // Check for resume file and use it if not empty
    ifstream resumeFile("resume.txt");
    vector<string> profile_lists;
    string line;
    bool usingResumeFile = false;

    while (getline(resumeFile, line))
    {
        if (!line.empty())
        {
            profile_lists.emplace_back(line);
            total_profiles++;
            usingResumeFile = true;
        }
    }
    resumeFile.close();

    // If using resume file, set start position to 1
    if (usingResumeFile)
    {
        start = 1;
    }

    // If not using resume file, read from the provided profile list file
    if (!usingResumeFile && !profile_list.empty())
    {
        file.open(profile_list.c_str());
        if (!file.is_open())
        {
            cout << red << "Error opening file" << def << endl;
            exit(1);
        }

        while (file >> profile)
        {
            profile_lists.emplace_back(profile);
            total_profiles++;
        }
        // Rewind file
        file.clear();
        file.seekg(0);
    }

    ofstream resumeFileOut("resume.txt"); // Open the resume file for writing

    while (!profile_lists.empty() || !interrupted)
    {
        if (interrupted)
        {
            cout << red << "Interrupt signal received, saving remaining profiles..." << def << endl;
            for (const auto& profile : profile_lists)
            {
                resumeFileOut << profile << endl;
            }
            resumeFileOut.close();
            break;
        }

        profile = getRandomAndRemove(profile_lists);
        cout << magenta << endl << "Item #" << cyan << " [" << counter << "/" << total_profiles << "]" << magenta <<
            ": " << profile << def << endl << endl;
        if (counter >= start)
        {
            int result = processProfile(profile);
            if (result == 0)
            {
                cout << cyan << "User " << profile << " downloaded successfully" << def << endl;
                success++;
            }
            else if (result == 1024)
            {
                cout << red << "User " << profile << " download failed" << def << endl;
                continue;
            }
            else
            {
                cout << red << "User " << profile << " download failed" << def << endl;
                /* Write a prompt if to retry */
                cout << "Error code: " << result << endl;
                cout << "Retry? (Retry(r) / Skip (s))" << endl;
                string prompt;
                cin >> prompt;

                while (prompt != "s" || result != 0)
                {
                    if (prompt == "r")
                    {
                        result = processProfile(profile);
                    }
                    else if (prompt == "s")
                    {
                        break;
                    }
                    if (result != 0)
                    {
                        cout << red << "User " << profile << " download failed" << def << endl;
                        /* Write a prompt if to retry */
                        cout << "Error code: " << result << endl;
                        cout << "Retry? (Retry(r) / Skip (s))" << endl;
                        cin >> prompt;
                    }
                    else
                    {
                        cout << cyan << "User " << profile << " downloaded successfully" << def << endl;
                        success++;
                        break;
                    }
                }
                failed++;
            }
            sleep_time = distribute(generator);

            cout << yellow << "Sleeping before next profile download: " << sleep_time << " seconds" << def << endl;
            sleep(sleep_time);
        }
        else
        {
            cout << yellow << "User " << profile << " download skipped" << def << endl;
            // Implement writing to log
            skipped++;
        }
        counter++;
    }

    resumeFileOut.close();

    // Delete the resume file if program finished normally
    if (!interrupted)
    {
        remove("resume.txt");
    }

    cout << magenta << "Success: " << success << endl << red << "Failed: " << failed << endl << yellow << "Skipped: " << skipped << endl
         << cyan << "Total number of tasks processed: " << counter << def << endl;

    return 0;
}

// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <..\DiligentTools\ThirdParty\imgui\imgui.h>

#include <Log.hpp>

#include <corecrt_malloc.h>
#include <cctype>

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.
namespace be::gui::test
{
    struct ExampleAppConsole
    {
        char                  InputBuf[256];
        ImVector<char*>       Items;
        ImVector<const char*> Commands;
        ImVector<char*>       History;
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        bool                  exit_command;

        vector<const char*> command_names = { "save", "load", "exit", "help", "clear", "god", "fov", "info", "free_camera", "bind"
                                                   "cpu_info", "camera_info", "fps_info", "msaa", "msaa_sample_count", "anisotropic_filtering" };

        ExampleAppConsole()
        {
            ClearLog();
            memset(InputBuf, 0, sizeof(InputBuf));
            HistoryPos = -1;

            Commands.push_back("help");
            Commands.push_back("history");
            Commands.push_back("clear");
            Commands.push_back("classify");  // "classify" is only here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
            // TODO: many more commands
            // Commands: pre-compile names and set up / syncronized run-time values

            AutoScroll = true;
            ScrollToBottom = false;

            exit_command = false;

            // non-sync writing log to console
            fs::path log_file_name_full = "user//logs//log.txt";
            std::fstream file(log_file_name_full);

            string line;
            if (file.is_open())
            {
                string line;
                while (std::getline(file, line))
                {
                    // using printf() in all tests for consistency
                    // TODO: sync with spdlog
                    AddLog(line.c_str());
                }

                file.close();
            }
        }

        ~ExampleAppConsole()
        {
            ClearLog();

            //for (int i = 0; i < History.Size; i++)
              //  delete History[i];

            History.clear();
        }

        // Portable helpers
        static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
        static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
        static char* Strdup(const char* str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
        static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

        void    ClearLog()
        {
            for (int i = 0; i < Items.Size; i++)
                delete Items[i];

            Items.clear();
        }

        template<typename... Args>
        void    AddLog(Args&&... args)
        {
            std::ostringstream strstream1;
            (strstream1 << ... << std::forward<Args>(args));

            char buf[1024];
            auto x = strstream1.str();
            auto y = x.c_str(); // Only this way, otherwise (strstream1.str().c_str()) just would look like '?????????' in the console

            strncpy_s(buf, y, sizeof(buf));
            buf[IM_ARRAYSIZE(buf) - 1] = 0;

            Items.push_back(Strdup(buf));
        }

        void    Draw(const char* title, bool* p_open = (bool*)false)
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(1366.f, 768.f / 2.f), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoDecoration))
            {
                ImGui::End();
                return;
            }

            // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
            // Here we create a context menu only available from the title bar.
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Close Console"))
                    *p_open = false;

                ImGui::EndPopup();
            }

            //ImGui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
            //ImGui::TextWrapped("Enter 'help' for help, press TAB to use text completion.");

            // TODO: display items starting from the bottom
            /*
            if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
            if (ImGui::SmallButton("Add Dummy Warn")) { AddLog("[warn] something went wrong"); } ImGui::SameLine();
            if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went really wrong"); } ImGui::SameLine();
            if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
            bool copy_to_clipboard = false;//= ImGui::SmallButton("Copy");
            */
            //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

            //ImGui::Separator();

            // Options menu
            /*
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Options, Filter
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");

            Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
            ImGui::Separator();
            */

            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
            /*
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) 
                    ClearLog();

                ImGui::EndPopup();
            }
            */

            // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
            // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
            // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
            //     ImGuiListClipper clipper(Items.Size);
            //     while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
            // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            //if (copy_to_clipboard)
              //  ImGui::LogToClipboard();

            //for (int i = 0; i < Items.Size; i++)
            for(auto& item : Items)
            {
                //const char* item = i;
                if (!Filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
                bool pop_color = false;

                for (auto x : command_names)
                    if (Stricmp(item, x) == 0)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
                        pop_color = true;
                    }

                if (strstr(item, "[error]") || strstr(item, "Unknown command")) 
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.0f, 0.0f, 1.0f)); 
                    pop_color = true; 
                }
#ifdef BE_DEBUG
                else if (strstr(item, "[debug]")) 
                { 
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.8f, 0.5f, 1.0f)); 
                    pop_color = true; 
                }
#endif
                else if (strstr(item, "[warning]")) 
                { 
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.0f, 1.0f)); 
                    pop_color = true; 
                }

                ImGui::TextUnformatted(item);
                if (pop_color)
                    ImGui::PopStyleColor();
            }

            //if (copy_to_clipboard)
              //  ImGui::LogFinish();

            auto& io = ImGui::GetIO();
            
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            
            ScrollToBottom = false;

            ImGui::PopStyleVar();
            ImGui::EndChild();
            ImGui::Separator();

            // Command-line
            bool reclaim_focus = false;
            if (ImGui::InputText("", InputBuf, sizeof(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
            {
                Strtrim(InputBuf);

                if (InputBuf[0])
                    ExecCommand(InputBuf);

                strcpy_s(InputBuf, "");
                reclaim_focus = true;
            }

            // Auto-focus on window apparition
            ImGui::SetItemDefaultFocus();
            if (reclaim_focus)
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

            ImGui::End();
        }

        void ExecCommand(const char* command_line)
        {
            AddLog(command_line);

            // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
            HistoryPos = -1;
            for (int i = History.Size - 1; i >= 0; i--)
                if (Stricmp(History[i], command_line) == 0)
                {
                    delete (History[i]);
                    History.erase(History.begin() + i);
                    break;
                }

            History.push_back(Strdup(command_line));


            auto pop_color = false;

            // Process command

            if (Stricmp(command_line, "clear") == 0)
                ClearLog();
            else if (Stricmp(command_line, "exit") == 0)
                exit_command = true;
            else if (Stricmp(command_line, command_names[3]) == 0)
            {
                AddLog("Commands:");

                for (auto& command : Commands)
                    AddLog("- ", command);

                //for (int i = 0; i < Commands.Size; i++)
                  //  AddLog("- ", Commands[i]);
            }
            
            else if (Stricmp(command_line, "history") == 0)
            {
                int first = History.Size - 10;

                for (int i = first > 0 ? first : 0; i < History.Size; i++)
                    AddLog(i, " : ", History[i]);
            }
            else
                AddLog("Unknown command: ", command_line);

            // On command input, we scroll to bottom even if AutoScroll==false
            ScrollToBottom = true;
        }

        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
        {
            ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
            return console->TextEditCallback(data);
        }

        int TextEditCallback(ImGuiInputTextCallbackData* data)
        {

            switch (data->EventFlag)
            {
                case ImGuiInputTextFlags_CallbackCompletion:
                {
                    // Example of TEXT COMPLETION

                    // if input is empty don't suggest any commands
                    if (Stricmp(InputBuf, "") == 0)
                        break;

                    // Locate beginning of current word
                    const char* word_end = data->Buf + data->CursorPos;
                    const char* word_start = word_end;
                    while (word_start > data->Buf)
                    {
                        const char c = word_start[-1];
                        if (c == ' ' || c == '\t' || c == ',' || c == ';')
                            break;

                        word_start--;
                    }

                    // Build a list of candidates
                    ImVector<const char*> candidates;
                    
                    for(auto& i : Commands)
                        if (Strnicmp(i, word_start, (int)(word_end - word_start)) == 0)
                            candidates.push_back(i);

                    //for (int i = 0; i < Commands.Size; i++)
                      //  if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                        //    candidates.push_back(Commands[i]);

                    if (candidates.Size == 0)
                    {
                        // No match
                        AddLog("No match for ", "-", word_start);
                    }
                    else if (candidates.Size == 1)
                    {
                        // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0]);
                        data->InsertChars(data->CursorPos, " ");
                    }
                    else
                    {
                        // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                        int match_len = (int)(word_end - word_start);

                        for (;;)
                        {
                            auto c = 0;
                            auto all_candidates_matches = true;
                            for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                                if (i == 0)
                                    c = toupper(candidates[i][match_len]);
                                else if (c == 0 || c != toupper(candidates[i][match_len]))
                                    all_candidates_matches = false;

                            if (!all_candidates_matches)
                                break;

                            match_len++;
                        }

                        if (match_len > 0)
                        {
                            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                            data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                        }

                        // List matches
                        AddLog("Possible matches:");

                        for (auto& i : candidates)
                            AddLog("- ", i);
                        //for (int i = 0; i < candidates.Size; i++)
                        //    AddLog("- ", candidates[i]);
                    }

                    break;
                }
                case ImGuiInputTextFlags_CallbackHistory:
                {
                    // Example of history
                    const int prev_history_pos = HistoryPos;
                    if (data->EventKey == ImGuiKey_UpArrow)
                    {
                        if (HistoryPos == -1)
                            HistoryPos = History.Size - 1;
                        else if (HistoryPos > 0)
                            HistoryPos--;
                    }
                    else if (data->EventKey == ImGuiKey_DownArrow)
                    {
                        if (HistoryPos != -1)
                            if (++HistoryPos >= History.Size)
                                HistoryPos = -1;
                    }

                    // A better implementation would preserve the data on the current input line along with cursor position.
                    if (prev_history_pos != HistoryPos)
                    {
                        const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                        data->DeleteChars(0, data->BufTextLen);
                        data->InsertChars(0, history_str);
                    }
                }
            }
            return 0;
        }
    };

    static void ShowExampleAppConsole(bool* p_open)
    {
        static ExampleAppConsole console;
        console.Draw("Example: Console", p_open);
    }
}
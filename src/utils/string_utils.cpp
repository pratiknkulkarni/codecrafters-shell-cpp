//
// Created by pnkulkarni on 11/12/25.
//

#include "string_utils.h"

#include <iostream>
#include<vector>


namespace utils {
    std::vector<std::string> tokenize(std::string_view line) {
        enum class State {
            NORMAL, // Outside any quotes
            IN_SINGLE_QUOTE, // Inside ''
            IN_DOUBLE_QUOTE, // Inside ""
            ESCAPED_NORMAL, // After \ outside quotes
            ESCAPED_DOUBLE // After \ inside double quotes
        };

        std::vector<std::string> tokens;
        std::string current_token;
        State state = State::NORMAL;

        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];

            switch (state) {
                case State::NORMAL:
                    if (c == '\'') {
                        state = State::IN_SINGLE_QUOTE;
                    } else if (c == '"') {
                        state = State::IN_DOUBLE_QUOTE;
                    } else if (c == '\\') {
                        state = State::ESCAPED_NORMAL;
                    } else if (std::isspace(static_cast<unsigned char>(c))) {
                        if (!current_token.empty()) {
                            tokens.push_back(std::move(current_token));
                            current_token.clear();
                        }
                    } else {
                        current_token += c;
                    }
                    break;

                case State::IN_SINGLE_QUOTE:
                    if (c == '\'') {
                        state = State::NORMAL;
                    } else {
                        current_token += c;
                    }
                    break;

                case State::IN_DOUBLE_QUOTE:
                    if (c == '"') {
                        state = State::NORMAL;
                    } else if (c == '\\') {
                        state = State::ESCAPED_DOUBLE;
                    } else {
                        current_token += c;
                    }
                    break;

                case State::ESCAPED_NORMAL:
                    current_token += c;
                    state = State::NORMAL;
                    break;

                case State::ESCAPED_DOUBLE:
                    current_token += c;
                    state = State::IN_DOUBLE_QUOTE;
                    break;
            }
        }

        if (!current_token.empty()) {
            tokens.push_back(std::move(current_token));
        }

        return tokens;
    }

    std::vector<std::string> split(std::string_view str, char delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != std::string_view::npos) {
            tokens.emplace_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }

        tokens.emplace_back(str.substr(start));
        return tokens;
    }
}

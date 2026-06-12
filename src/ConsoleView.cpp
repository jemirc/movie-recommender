#include "ConsoleView.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <tabulate/table.hpp>

namespace
{
using tabulate::Color;
using tabulate::FontAlign;
using tabulate::FontStyle;
using tabulate::Table;
using Row = Table::Row_t;

std::string toText(int value)
{
    return std::to_string(value);
}

void styleBaseTable(Table &table, std::size_t columnCount)
{
    table.format()
        .border_color(Color::blue)
        .corner_color(Color::blue)
        .padding_left(1)
        .padding_right(1);

    for (std::size_t column = 0; column < columnCount; ++column)
    {
        table.column(column).format().multi_byte_characters(true);
        table[0][column]
            .format()
            .font_color(Color::cyan)
            .font_align(FontAlign::center)
            .font_style({FontStyle::bold})
            .multi_byte_characters(true);
    }
}

void rightAlignColumns(Table &table, const std::vector<std::size_t> &columns)
{
    for (std::size_t column : columns)
    {
        table.column(column).format().font_align(FontAlign::right);
        table[0][column].format().font_align(FontAlign::center);
    }
}

void printTable(Table &table)
{
    std::cout << table << std::endl;
}
}

namespace ConsoleView
{
std::string formatScore(double score)
{
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << score;
    return output.str();
}

void printMovieTable(const std::vector<const Movie *> &movies)
{
    Table table;
    table.add_row(Row{"ID", "제목", "장르", "개봉 연도", "평균 평점", "평점 수"});

    for (const Movie *movie : movies)
    {
        table.add_row(Row{
            toText(movie->getId()),
            movie->getTitle(),
            movie->getGenre(),
            toText(movie->getReleaseYear()),
            formatScore(movie->getAverageRating()),
            toText(movie->getRatingCount()) + "건"});
    }

    styleBaseTable(table, 6);
    rightAlignColumns(table, {0, 3, 4, 5});
    printTable(table);
}

void printUserTable(const std::vector<const User *> &users)
{
    Table table;
    table.add_row(Row{"ID", "이름", "이메일"});

    for (const User *user : users)
    {
        table.add_row(Row{
            toText(user->getId()),
            user->getName(),
            user->getEmail()});
    }

    styleBaseTable(table, 3);
    rightAlignColumns(table, {0});
    printTable(table);
}

void printRatingTable(const std::vector<const Rating *> &ratings)
{
    Table table;
    table.add_row(Row{"사용자 ID", "영화 ID", "점수"});

    for (const Rating *rating : ratings)
    {
        table.add_row(Row{
            toText(rating->getUserId()),
            toText(rating->getMovieId()),
            formatScore(rating->getScore())});
    }

    styleBaseTable(table, 3);
    rightAlignColumns(table, {0, 1, 2});
    printTable(table);
}

void printRecommendationTable(const std::vector<std::pair<const Movie *, double>> &recommendations)
{
    Table table;
    table.add_row(Row{"순위", "ID", "제목", "장르", "평균 평점", "평점 수", "추천 점수"});

    int rank = 1;
    for (const auto &[movie, score] : recommendations)
    {
        table.add_row(Row{
            toText(rank),
            toText(movie->getId()),
            movie->getTitle(),
            movie->getGenre(),
            formatScore(movie->getAverageRating()),
            toText(movie->getRatingCount()) + "건",
            formatScore(score)});
        rank++;
    }

    styleBaseTable(table, 7);
    rightAlignColumns(table, {0, 1, 4, 5, 6});
    printTable(table);
}

void printGenreStatisticsTable(const std::map<std::string, GenreStatistics> &statisticsByGenre)
{
    Table table;
    table.add_row(Row{"장르", "영화 수", "평점 수", "평균 평점"});

    for (const auto &[genre, statistics] : statisticsByGenre)
    {
        table.add_row(Row{
            genre,
            toText(statistics.movieCount) + "편",
            toText(statistics.ratingCount) + "건",
            formatScore(statistics.averageRating)});
    }

    styleBaseTable(table, 4);
    rightAlignColumns(table, {1, 2, 3});
    printTable(table);
}

void printKeyValueTable(const std::vector<std::pair<std::string, std::string>> &rows)
{
    Table table;
    table.add_row(Row{"항목", "값"});

    for (const auto &[key, value] : rows)
    {
        table.add_row(Row{key, value});
    }

    styleBaseTable(table, 2);
    table.column(1).format().font_align(FontAlign::right);
    table[0][1].format().font_align(FontAlign::center);
    printTable(table);
}
}

#pragma once
#include <catch.hpp>
#include "comment.h"

TEST_CASE("Test case comment class") {
    SECTION("Create a empty comment") {
        Comment c;
        REQUIRE(c.name() == QString());
        REQUIRE(c.comment() == QString());
        REQUIRE(c.author() == QString());
        REQUIRE(c.description() == QString());
    }
    SECTION("Create a comment") {
        Comment c{
            QStringLiteral("name")
            , QStringLiteral("comment")
            , QStringLiteral("author")
            , QStringLiteral("desc")
        };
        REQUIRE(c.name() == QStringLiteral("name"));
        REQUIRE(c.comment() == QStringLiteral("comment"));
        REQUIRE(c.author() == QStringLiteral("author"));
        REQUIRE(c.description() == QStringLiteral("desc"));
    }
    SECTION("Check Equals comments") {
        Comment c1 {
            QStringLiteral("name")
            , QStringLiteral("comment")
            , QStringLiteral("author")
            , QStringLiteral("desc")
        };
        Comment c2 {
            QStringLiteral("name")
            , QStringLiteral("comment")
            , QStringLiteral("author")
            , QStringLiteral("desc")
        };
        REQUIRE(c1 == c2);
        REQUIRE_FALSE(c1 != c2);
        REQUIRE(c1 != Comment());
        REQUIRE(Comment() != c2);
        REQUIRE(Comment() == Comment());
    }
}

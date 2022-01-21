#pragma once
#include <run_tests/run_tests.h>
#include "model_comments.h"
#include "classes/json.h"

TEST_CASE("Test case tablecomment class") {
    project_info::ModelComments m;
    AssertEquals(m.columnCount(), 3);
    AssertTrue(m.insertRows(0, 1));
    AssertEquals(m.rowCount(), 1);

    SECTION("Check empty serialization") {
        auto const& s = m.serialize();
        AssertFalse(s.isEmpty());

        auto const& f = s.first().toObject();
        AssertTrue(json::getString(f, RT_STR("comment"), RT_STR("comment")).isEmpty());
        AssertTrue(json::getString(f, RT_STR("author"), RT_STR("author")).isEmpty());
        AssertFalse(json::getString(f, RT_STR("date"), RT_STR("date")).isEmpty());
    }
}

#pragma once
#include <run_tests/run_tests.h>
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/project_info_item/project_info_item.h"
#include "classes/base_model/items/project_info_item/model/comment.h"

class TestProjectInfoItem {

public:
    TestProjectInfoItem(ProjectInfoItem* info = nullptr)
        : m_item(info) {
    }
public:
    bool insertRows(int position, int rows, const QModelIndex& index) {
        return m_item->m_model.insertRows(position, rows, index);
    }
    void setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
        m_item->m_model.setData(index, value, role);
    }
    QVariant data(const QModelIndex &index, int role) {
        return m_item->m_model.data(index, role);
    }
    QModelIndex indexFromTable(int row, int column, const QModelIndex& parent = QModelIndex()) {
        return m_item->m_model.index(row, column, parent);
    }
    QJsonValue serializeFromTable() {
        return m_item->m_model.serialize();
    }

private:
    ProjectInfoItem* m_item = nullptr;
};

TEST_CASE("Test case for tableComment model") {
    BaseModel model;
    model.insertItem(0, new ProjectInfoItem());
    auto item = model.projectInfoItem();
    AssertNotNullptr(item);

    SECTION("Set/Get Customer") {
        item->setCustomer(RT_STR("String_test33"));
        AssertEquals(item->customer(), RT_STR("String_test33"));

        item->setCustomer(QString());
        AssertTrue(item->customer().isEmpty());
    }
    SECTION("Set/Get Location") {
        item->setLocation(RT_STR("Location"));
        AssertEquals(item->location(), RT_STR("Location"));

        item->setLocation({});
        AssertTrue(item->location().isEmpty());
    }
    SECTION("Set/Get ProjectNumber") {
        item->setProjectNumber(RT_STR("Number"));
        AssertEquals(item->projectNumber(), RT_STR("Number"));

        item->setProjectNumber({});
        AssertTrue(item->projectNumber().isEmpty());
    }
    SECTION("Set/Get SystemID") {
        item->setSystemID(RT_STR("systemID"));
        AssertEquals(item->systemID(), RT_STR("systemID"));

        item->setSystemID({});
        AssertTrue(item->systemID().isEmpty());
    }
    SECTION("Set/Get desctiption") {
        item->setDescription(RT_STR("desc"));
        AssertEquals(item->description(), RT_STR("desc"));

        item->setDescription({});
        AssertTrue(item->description().isEmpty());
    }
}
TEST_CASE("Test case for serialize/deserialize in project_info_item") {
    BaseModel model;
    model.insertItem(0, new ProjectInfoItem(), QModelIndex());
    auto fullItem = model.projectInfoItem();
    AssertNotNullptr(fullItem);

    TestProjectInfoItem infoTest(fullItem);

    fullItem->setCustomer(RT_STR("defaultCustomer"));
    fullItem->setLocation(RT_STR("defaultLocation"));
    fullItem->setProjectNumber(RT_STR("defaultProjectNumber"));
    fullItem->setSystemID(RT_STR("defaultSystemID"));
    fullItem->setDescription(RT_STR("defaultDescription"));

    infoTest.insertRows(0, 1, QModelIndex());
    infoTest.insertRows(1, 1, QModelIndex());

    auto const& index01 = infoTest.indexFromTable(0, 1);
    auto const& index02 = infoTest.indexFromTable(0, 2);
    auto const& index03 = infoTest.indexFromTable(0, 3);
    auto const& index04 = infoTest.indexFromTable(0, 4);

    auto const& index11 = infoTest.indexFromTable(1, 1);
    auto const& index12 = infoTest.indexFromTable(1, 2);
    auto const& index13 = infoTest.indexFromTable(1, 3);
    auto const& index14 = infoTest.indexFromTable(1, 4);

    infoTest.setData(index01, RT_STR("TestName"));
    infoTest.setData(index02, RT_STR("TestComment"));
    infoTest.setData(index03, RT_STR("TestAuthor"));
    infoTest.setData(index04, RT_STR("TestDesc"));

    infoTest.setData(index11, RT_STR("TestName1"));
    infoTest.setData(index12, RT_STR("TestComment1"));
    infoTest.setData(index13, RT_STR("TestAuthor1"));
    infoTest.setData(index14, RT_STR("TestDesc1"));

    QJsonObject jsonItem = {
            {RT_STR("customer"), fullItem->customer()}
          , {RT_STR("location"), fullItem->location()}
          , {RT_STR("number"), fullItem->projectNumber()}
          , {RT_STR("id"), fullItem->systemID()}
          , {RT_STR("desc"), fullItem->description()}
          , {RT_STR("comments"), infoTest.serializeFromTable()}};
    QJsonObject jsonFull = {{RT_STR("Project information"), jsonItem}};

    QJsonObject jsonItemNoValid = {
            {RT_STR("custom"), fullItem->customer()}
          , {RT_STR("loc"), fullItem->location()}
          , {RT_STR("numr"), fullItem->projectNumber()}
          , {RT_STR("id"), fullItem->systemID()}
          , {RT_STR("des"), fullItem->description()}
          , {RT_STR("comment"), infoTest.serializeFromTable()}};
    QJsonObject jsonFullNoValid = {{RT_STR("Project information"), jsonItemNoValid}};

    SECTION("serialize()") {
        AssertEquals(fullItem->serialize(), jsonItem);
    }
    SECTION("deserialize()") {
        AssertTrue(fullItem->deserialize(jsonFull));
    }
    SECTION("deserialize()") {
        AssertFalse(fullItem->deserialize(jsonFullNoValid));
    }
}
TEST_CASE("ProjeiInfoItem empty seilize/desirilaze tests") {
    BaseModel model;
    model.insertItem(0, new ProjectInfoItem());
    auto item = model.projectInfoItem();
    AssertNotNullptr(item);

        auto emptyItem = model.projectInfoItem();
        LOG_FATAL(emptyItem, "emptyItem cannot be null");
        TestProjectInfoItem infoTesteEmpty(emptyItem);

        infoTesteEmpty.insertRows(0, 1, QModelIndex());
        infoTesteEmpty.insertRows(1, 1, QModelIndex());

        auto const& ind01 = infoTesteEmpty.indexFromTable(0, 1, QModelIndex());
        auto const& ind02 = infoTesteEmpty.indexFromTable(0, 2, QModelIndex());
        auto const& ind03 = infoTesteEmpty.indexFromTable(0, 3, QModelIndex());
        auto const& ind04 = infoTesteEmpty.indexFromTable(0, 4, QModelIndex());
        auto const& ind11 = infoTesteEmpty.indexFromTable(1, 1, QModelIndex());
        auto const& ind12 = infoTesteEmpty.indexFromTable(1, 2, QModelIndex());
        auto const& ind13 = infoTesteEmpty.indexFromTable(1, 3, QModelIndex());
        auto const& ind14 = infoTesteEmpty.indexFromTable(1, 4, QModelIndex());

        infoTesteEmpty.setData(ind01, {});
        infoTesteEmpty.setData(ind02, {});
        infoTesteEmpty.setData(ind03, {});
        infoTesteEmpty.setData(ind04, {});

        infoTesteEmpty.setData(ind11, {});
        infoTesteEmpty.setData(ind12, {});
        infoTesteEmpty.setData(ind13, {});
        infoTesteEmpty.setData(ind14, {});

        emptyItem->setCustomer(RT_STR(""));
        emptyItem->setLocation(RT_STR(""));
        emptyItem->setProjectNumber(RT_STR(""));
        emptyItem->setSystemID(RT_STR(""));
        emptyItem->setDescription(RT_STR(""));

        QJsonObject jsonEmpty = {
             {RT_STR("customer"), emptyItem->customer()}
           , {RT_STR("location"), emptyItem->location()}
           , {RT_STR("number"), emptyItem->projectNumber()}
           , {RT_STR("id"), emptyItem->systemID()}
           , {RT_STR("desc"), emptyItem->description()}
           , {RT_STR("comments"), infoTesteEmpty.serializeFromTable()}};
        QJsonObject jsonFullempty = {{RT_STR("Project information"), jsonEmpty}};

        QJsonObject jsonEmptyNoValid = {
             {RT_STR("custom"), emptyItem->customer()}
           , {RT_STR("loc"), emptyItem->location()}
           , {RT_STR("numb"), emptyItem->projectNumber()}
           , {RT_STR("id"), emptyItem->systemID()}
           , {RT_STR("des"), emptyItem->description()}
           , {RT_STR("comment"), infoTesteEmpty.serializeFromTable()}};
        QJsonObject jsonFullemptyNoValid = {{RT_STR("Project information"), jsonEmptyNoValid}};

        SECTION("serialize()") {
            AssertEquals(emptyItem->serialize(), jsonEmpty);
        }
        SECTION("deserialize()") {
            AssertTrue(emptyItem->deserialize(jsonFullempty));
        }
        SECTION("deserialize()") {
            AssertFalse(emptyItem->deserialize(jsonFullemptyNoValid));
        }
}

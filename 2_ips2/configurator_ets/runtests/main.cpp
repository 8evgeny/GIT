#include <QApplication>
#include <run_tests/run_tests.h>
#include "classes/base_model/items/groups/groups_item_test.h"
#include "messages/dialogs/messages/messages_dialog_tests.h"
#include "classes/base_model/items/project_info_item/project_info_item_test.h"
#include "classes/base_model/items/project_info_item/model/model_comments_test.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RUN_TESTS()
}

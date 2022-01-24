#include "programmer_dialog.h"
#include "ui_programmer_dialog.h"
#include "programmer_instance.h"
#include "serializator.h"
#include "host_checker.h"
#include "sender_data.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "programmer/client_server/server/islp_server.h"
#include "dialogs/send_configuration/send_configuration.h"

ProgrammerDialog::ProgrammerDialog(StationsItem* stationsItem, QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint)    
    , ui(new Ui::ProgrammerDialog)
    , m_viewError(parent)
    , m_serilizator()
    , m_senderData()
    , m_checker()
{
    ui->setupUi(this);
    setDataForModel(stationsItem, &m_model);
    ProgrammerItemInstance(ui, &m_model, &m_logs);
    m_serilizator = new Serializator(&m_model, m_item);
    m_senderData = new SenderData(ui, &m_model, &m_logs, m_viewError, m_serilizator);
    m_checker = new HostChecker(ui, &m_model, &m_logs, m_senderData);
    QObject::connect(ui->d_device_polling, SIGNAL(clicked(bool)), m_checker, SLOT(checkableHosts(bool)));
}
ProgrammerDialog::~ProgrammerDialog()
{
    delete ui;    
    delete m_serilizator;
    delete m_senderData;
    delete m_checker;
}
void ProgrammerDialog::setDataForModel(StationsItem* stationsItem, ModelProgrammer* model) noexcept
{
    QList<StationProgrammer*> stations;
    std::transform(stationsItem->cbegin(), stationsItem->cend(), std::back_inserter(stations), [this] (auto item) {
        m_item = dynamic_cast<StationItem*>(item);
        return new StationProgrammer(m_item);
    });
    model->setItems(stations);    
}

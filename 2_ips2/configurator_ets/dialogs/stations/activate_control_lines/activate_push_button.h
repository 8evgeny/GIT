#pragma once
#include <QPushButton>
#include "include/view.h"
#include "main/classes/base_model/items/station_item/model_keys/model_keys.h"

class ActivatePushButton final : public QPushButton {

   using inherited  = QPushButton;
   using class_name = ActivatePushButton;

public:
    explicit ActivatePushButton(ModelKeys* model, QPushButton* parent = nullptr)
        : inherited(parent)
        , m_model(model) {        
    }
    virtual ~ActivatePushButton() = default;


    void setModel(ModelKeys* model) noexcept {
        m_model = model;
    }
private:
    ModelKeys* m_model = nullptr;
};

#ifndef REC_GEP_VIEW_HPP
#define REC_GEP_VIEW_HPP

#include "application.hpp"
#include "button.hpp"
#include "dropdown.hpp"
#include "list.hpp"
#include "rec_gep_model.hpp"
#include "setvalue.hpp"
#include "statictext.hpp"
#include "text.hpp"
#include <string>

class RecGepView
{
    RecGepModel* _model = nullptr;

    StaticText* statusLabel = nullptr;
    Text* nameInput = nullptr;
    SetValue* sizeInput = nullptr;
    SetValue* countInput = nullptr;
    Dropdown* recDropdown = nullptr;
    List* machineList = nullptr;

    std::string _saved_machine;

public:
    void bind(RecGepModel* model) { _model = model; }
    void create_widgets(Lib* lib);
    void set_status(const std::string& msg);
    void refresh_all();

    std::string name_text() const;
    int size_value() const;
    int count_value() const;
    int selected_rec_index() const;
    int selected_machine_row() const;

    Button* addRecButton = nullptr;
    Button* insertButton = nullptr;
    Button* removeButton = nullptr;
};

#endif

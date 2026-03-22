/****************************************************************************
** Meta object code from reading C++ file 'QTWidgetsApp.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../QTWidgetsApp.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QTWidgetsApp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12QTWidgetsAppE_t {};
} // unnamed namespace

template <> constexpr inline auto QTWidgetsApp::qt_create_metaobjectdata<qt_meta_tag_ZN12QTWidgetsAppE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QTWidgetsApp",
        "onMicrophoneButtonClicked",
        "",
        "onTextInputButtonClicked",
        "onSwitchAiButtonClicked",
        "onSendTextButtonClicked",
        "onComputerManagerAiButtonClicked",
        "UseChatAI",
        "std::string",
        "user_input",
        "UseComputerManagerAI"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onMicrophoneButtonClicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTextInputButtonClicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSwitchAiButtonClicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSendTextButtonClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onComputerManagerAiButtonClicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'UseChatAI'
        QtMocHelpers::SlotData<void(const std::string &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'UseComputerManagerAI'
        QtMocHelpers::SlotData<void(const std::string &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QTWidgetsApp, qt_meta_tag_ZN12QTWidgetsAppE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QTWidgetsApp::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QTWidgetsAppE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QTWidgetsAppE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12QTWidgetsAppE_t>.metaTypes,
    nullptr
} };

void QTWidgetsApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QTWidgetsApp *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onMicrophoneButtonClicked(); break;
        case 1: _t->onTextInputButtonClicked(); break;
        case 2: _t->onSwitchAiButtonClicked(); break;
        case 3: _t->onSendTextButtonClicked(); break;
        case 4: _t->onComputerManagerAiButtonClicked(); break;
        case 5: _t->UseChatAI((*reinterpret_cast<std::add_pointer_t<std::string>>(_a[1]))); break;
        case 6: _t->UseComputerManagerAI((*reinterpret_cast<std::add_pointer_t<std::string>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *QTWidgetsApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QTWidgetsApp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QTWidgetsAppE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QTWidgetsApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP

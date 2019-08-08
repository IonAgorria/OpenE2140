//
// Created by Ion Agorria on 10/06/19
//
#ifndef OPENE2140_COMPONENT_H
#define OPENE2140_COMPONENT_H

#include "engine/core/to_string.h"
#include "engine/core/common.h"

/**
 * This macro passes each component methods to provided macro
 */
#define COMPONENT_METHODS(MACRO_METHOD) \
    MACRO_METHOD(componentsUpdate, update) \
    MACRO_METHOD(componentsSetup, setup) \
    MACRO_METHOD(componentsSimulationChanged, simulationChanged)

/**
 * Template for component method declaration
 */
#define COMPONENT_METHOD_DECLARATION(BASE_METHOD, COMPONENT_METHOD) \
    void COMPONENT_METHOD();

/**
 * Template for virtual component method forwarder
 */
#define COMPONENT_METHOD_FORWARD_VIRTUAL(BASE_METHOD, COMPONENT_METHOD) \
    virtual void BASE_METHOD() = 0;

/**
 * Wrapper for forwarding each components methods assigned to binder
 */
#define COMPONENT_METHOD_FORWARD(BASE_METHOD, COMPONENT_METHOD) \
    void BASE_METHOD() override { \
        (Components::COMPONENT_METHOD(), ...); \
    }

/**
 * Assigns provided components to Base and creates a new class
 */
template<typename Base, typename Derived, typename ... Components>
class ComponentBinder: public Base, public Components... {
public:
    /**
     * Constructor that calls base and each component with pointer to this class
     */
    ComponentBinder(): Base(), Components(static_cast<Derived*>(this))... {};

    /*
     * Mass forward methods
     */
    COMPONENT_METHODS(COMPONENT_METHOD_FORWARD)
};

/**
 * Macro for component body definition without component methods or class
 *
 * T_BASE is the class type that will specify this component base pointer
 * If the class this component is attached to is not convertible to T_BASE a compilation error will occur due to static casting
 * T_COMPONENT name of this component type
 */
#define CLASS_COMPONENT_BODY(T_BASE, T_COMPONENT) \
protected: \
    /** Internal pointer to itself with base type */ \
    T_BASE* base; \
public: \
    /** Constructs the component and receives pointer to base which belongs to */ \
    explicit T_COMPONENT(T_BASE* base): base(base) { construction(); }; \
    /** Set type name */ \
    TYPE_NAME(T_COMPONENT) \
    /** Disable copy and move */ \
    NON_COPYABLE_NOR_MOVABLE(T_COMPONENT) \
    /** Attempts to cast base dynamically */ \
    template<typename T> T* castBase() { return dynamic_cast<T*>(base); };

/**
 * Macro for component class definition with component methods
 *
 * T_BASE is the class type that will specify this component base pointer
 * If the class this component is attached to is not convertible to T_BASE a compilation error will occur due to static casting
 * T_COMPONENT name of this component type
 */
#define CLASS_COMPONENT(T_BASE, T_COMPONENT) \
class T_COMPONENT { \
    CLASS_COMPONENT_BODY(T_BASE, T_COMPONENT) \
    /** Called on construction */ \
    void construction(); \
    /** Destructor */ \
    virtual ~T_COMPONENT(); \
    /** Creates declarations of component methods */ \
    COMPONENT_METHODS(COMPONENT_METHOD_DECLARATION)

/**
 * Macro for component class definition with empty constructor/destructor
 */
#define CLASS_COMPONENT_DEFAULT(T_COMPONENT) \
    void T_COMPONENT::construction() {}; \
    T_COMPONENT::~T_COMPONENT() = default;

/**
 * Macro for component dynamic casting from entity
 */
#define GET_COMPONENT(OBJECT, T_COMPONENT) \
    (dynamic_cast<T_COMPONENT*>(OBJECT))

#endif //OPENE2140_COMPONENT_H

/**
 * Origin:
 * https://github.com/koralkashri/CommonTemplateBase
 */

#ifndef BANKMANAGEMENT_TEMPLATE_BASE_INTERFACE_IMPL_H
#define BANKMANAGEMENT_TEMPLATE_BASE_INTERFACE_IMPL_H

// A generic interface implementation
template <typename Interface, typename T>
class template_base_interface_implementation : public Interface {
public:
    explicit template_base_interface_implementation(T& impl_obj) : m_impl_obj(impl_obj) {}

    // Interface should be fully implemented here
    // region [Interface Implementation Begin]
    // endregion [Interface Implementation End]

private:
    T& m_impl_obj; // Contain the target specialized class instance
};

#endif //BANKMANAGEMENT_TEMPLATE_BASE_INTERFACE_IMPL_H
#ifndef BANKMANAGEMENT_SHADOW_UPDATES_H
#define BANKMANAGEMENT_SHADOW_UPDATES_H

#include <thread>
#include "../db/db_management.h"

class shadow_updates {
public:
    shadow_updates();
    ~shadow_updates();

private:
    std::thread monthly_cash_updater_t;
    db_management db;

    static void monthly_cash_updater(shadow_updates &ref);

    [[nodiscard]] bool is_month_updated() const;
    void wait_for_update_time() const;

    void single_time_transactions_handler(std::vector<transaction> &&tancs);
    void continues_transactions_handler(std::vector<transaction> &&tancs);
    void active_transactions_handler(std::vector<transaction> &&tancs);
    void inactive_transactions_handler(std::vector<transaction> &&tancs);
};


#endif //BANKMANAGEMENT_SHADOW_UPDATES_H

//
// Created by jonas on 14.12.21.
//

#ifndef GAMEBOYCOMPILER_DELIVERYVISITOR_HPP
#define GAMEBOYCOMPILER_DELIVERYVISITOR_HPP

#include "DeliveryBaseVisitor.h"

class DeliveryVisitor : public delivery::DeliveryBaseVisitor {
    public:
        antlrcpp::Any visitDelivery(delivery::DeliveryParser::DeliveryContext *ctx) override;

        antlrcpp::Any visitOrder(delivery::DeliveryParser::OrderContext *ctx) override;

        antlrcpp::Any visitPrice(delivery::DeliveryParser::PriceContext *ctx) override;
};

#endif //GAMEBOYCOMPILER_DELIVERYVISITOR_HPP

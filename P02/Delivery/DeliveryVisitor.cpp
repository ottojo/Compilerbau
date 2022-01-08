//
// Created by jonas on 14.12.21.
//

#include "DeliveryVisitor.hpp"

antlrcpp::Any DeliveryVisitor::visitOrder(delivery::DeliveryParser::OrderContext *ctx) {
    return visitPrice(ctx->price()).as<float>();
}

antlrcpp::Any DeliveryVisitor::visitPrice(delivery::DeliveryParser::PriceContext *ctx) {
    auto text = ctx->FLOAT()->getText();
    float f = std::stof(text);
    return f;
}

antlrcpp::Any DeliveryVisitor::visitDelivery(delivery::DeliveryParser::DeliveryContext *ctx) {
    float sum = 0;
    for (const auto &order: ctx->order()) {
        sum += visitOrder(order).as<float>();
    }
    return sum;
}

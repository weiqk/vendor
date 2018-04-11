#pragma once
#if !defined(__TRADEONE_MAIN_DEF_H__)
#define __TRADEONE_MAIN_DEF_H__

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>
#include <cstdint>

enum enum_order_status
{
    // 0 = New; 
    // 1 = Partially filled; 
    // 2 = Filled; 
    // 4 = Cancelled(When an order is partially filled, "Cancelled" is returned signifying Tag 151: LeavesQty is cancelled and will not be subsequently filled); 
    // 8 = Rejected; 
    // C = Expired.
    order_status_new = 0,
    order_status_partially_filled = 1,
    order_status_filled = 2,
    order_status_cancelled = 4,
    order_status_rejected = 8,
    order_status_expired = 'C',
};

//ExecType 150 Y F Char
enum enum_order_ExecType
{
    // 0 = New; 
    // 4 = Canceled; 
    // 5 = Replace; 
    // 8 = Rejected; 
    // C = Expired; 
    // F = Trade; 
    // I = Order Status. 

    order_exectype_new = 0,
    order_exectype_canceled = 4,
    order_exectype_replace = 5,
    order_exectype_rejected = 8,
    order_exectype_expired = 'C',
    order_exectype_trade = 'F',
    order_exectype_order_status = 'I',
};

typedef enum
{
    qoute_status_none=0,
    qoute_status_start=1,
    qoute_status_pause=2,

}t_qoute_status;

typedef enum
{
    position_none = 0,
    position_today = 1,
    position_yestaday = 2,

}t_position_type;

typedef enum
{
    side_none = 0,
    side_buy = 1,
    side_sell = 2,
} t_order_side;

enum enum_order_action
{
    action_none = 0,
    /* 插入新订单 */
    action_insert = 1,
    /* 取消订单 */
    action_cancel = 2,
    /* 修改订单 */
    action_modify = 3,
};


enum enum_order_type
{
    ordtype_none = 0,
    /* market 市价单 */
    ordtype_market = 1,
    /* 限价单 */
    ordtype_limit = 2,
    /* stop order */
    ordtype_stop = 3,
};

typedef struct
{
    double price_buy[10];
    int vol_buy[10];
} type_tick;


// 订单类型 用于下单或撤单. 5.4.4. New Order Single (MsgType(35)=D)
typedef struct
{
    char ClOrdID[32]; // 11 Y String Unique identifier for the order, allocated by the client.        
    char Symbol[16]; // 55 Y Long Instrument identificators are provided by Spotware.
    
    // 订单方向 t_order_side
    // 1 = Buy;
    // 2 = Sell.
    uint8_t Side; // 54 Y 1 Integer   
    uint32_t time;
    uint16_t timemsec;
    uint32_t TransactTime; // 60 Y Timestamp Client generated request time.
    uint32_t OrderQty; // 38 Y Qty Number of shares ordered.This represents the number of shares for equities or based on normal convention the number of contracts for options, futures, convertible bonds, etc.Maximum precision is 0.01 (Prior to FIX 4.2 this field was of type int)
    
    // 订单动作. enum_order_action
    // 1 = 插入新订单
    // 2 = 取消订单
    // 3 = 修改订单
    uint8_t action;

    // 订单类型
    // 1 = Market, the Order will be processed by 'Immediate Or Cancel' scheme (see TimeInForce(59) : IOC);
    // 2 = Limit, the Order will be processed by 'Good Till Cancel' scheme(see TimeInForce(59) : GTC);
    // 3 = Stop, the Order will be processed by 'Good Till Cancel' scheme(see TimeInForce(59) : GTC).
    uint8_t  OrdType; //40 Y 1, 2 or 3 Char
    double Price; // 44 N Price The worst client price that the client will accept.Required when OrdType = 2, in which case the order will not fill unless this price can be met.
    double StopPx; // 99 N Price Price that triggers a Stop order. Required when OrdType = 3, in which case the order will not fill unless this price can be met.
    
    //TimeInForce will be detected automaticly depending on OrdType(40) and ExpireTime(126): 
    // 1 = Good Till Cancel(GTC), will be used only for Limit and Stop Orders(see OrdType(40)) only if ExpireTime(126) is not defined 
    // 6 = Good Till Date(GTD), will be used only for Limit and Stop Orders(see OrdType(40)) only if ExpireTime(126) is defined; 
    // 3 = Immediate Or Cancel(IOC), will be used only for Market Orders (see OrdType(40));
    uint8_t TimeInForce; // 59 N String Deprecated, this value will be ignored.
    uint32_t ExpireTime; // 126 N 20140215 -07:24:55 Timestamp Expire Time in YYYYMMDD-HH:MM:SS format.If is assigned then the Order will be processed by 'Good Till Date' scheme(see TimeInForce : GTD).
    char PosMaintRptID[32]; // 721 N String Position ID, where this order should be placed.If not set, new position will be created, it’s id will be returned in ExecutionReport(8) message.
    char Designation[128]; // 494 N String Client custom order label.
} type_order;


// 订单回报.
typedef struct 
{
    char OrderID[32]; // 37 Y String cTrader order id.
    char ClOrdID[32];// 11 N String Unique identifier for the order, allocated by the client.
    uint32_t TotNumReports; // 911 N Integer Total number or reports returned in response to a OrderMassStatusRequest.
    
    // 订单执行类型 enum_order_ExecType
    // 0 = New; 
    // 4 = Canceled; 
    // 5 = Replace; 
    // 8 = Rejected; 
    // C = Expired; 
    // F = Trade; 
    // I = Order Status. 
    uint8_t ExecType; // 150 Y F Char 

    // 订单状态 参考 enum_order_status
    // 0 = New; 
    // 1 = Partially filled; 
    // 2 = Filled; 
    // 8 = Rejected; 
    // 4 = Cancelled(When an order is partially filled, "Cancelled" is returned signifying Tag 151: LeavesQty is cancelled and will not be subsequently filled); 
    // C = Expired.
    uint8_t OrdStatus; // 39 Y 2 Char 
    char Symbol[16]; // 55 N Long Instrument identificators are  provided by Spotware.
    uint8_t Side; // 54 N 1 Integer 1 = Buy; 2 = Sell.
    uint32_t TransactTime;// 60 N Timestamp Time the transaction represented by this ExecutionReport occurred message(in UTC).
    uint8_t AvgPx;// 6 N Integer The price at which the deal was filled.For an IOC or GTD order, this is the VWAP(Volume Weighted Average Price) of the filled order.
    uint32_t OrderQty; // 38 N Qty Number of shares ordered.This represents the number of shares for equities or based on normal onvention the number of contracts for options, futures, convertible bonds, etc. (Prior to FIX 4.2 this field was of type int)
    uint32_t LeavesQty; // 151 N Qty The amount of the order still to be filled.This is a value between 0 (fully filled) and OrderQty(partially filled).
    uint32_t CumQty; // 14 N Qty The total amount of the order which has been filled.
    uint8_t OrdType; // 40 N 1 or 2 Char 1 = Market; 2 = Limit. 
    double Price; // 44 N Price If supplied in the NewOrderSingle, it is echoed back in this ExecutionReport.
    double StopPx; // 99 N Price If supplied in the NewOrderSingle, it is echoed back in this ExecutionReport.
    uint8_t TimeInForce;//59 N 3 String 1 = Good Till Cancel(GTC); 3 = Immediate Or Cancel(IOC); 6 = Good Till Date(GTD).
    uint32_t ExpireTime; // 126 N 20140215 - 07:24:55 Timestamp If supplied in the NewOrderSingle, it s echoed back in this ExecutionReport.
    char * Text; //58 N String Where possible, message to explain execution report.
    uint8_t OrdRejReason;  // 103 N 0 Integer 0 = OrdRejReason.BROKER_EXCHANGE_OPTION
    char PosMaintRptID[32]; // 721 N String Position ID.
    char Designation[128]; // 494 N String Client custom order label.
    char MassStatusReqID[32]; // 584 N String Unique ID of mass status request as assigned by the client.
    double AbsoluteTP; // 1000 N Price The absolute price at which Take Profit will be triggered.
    double RelativeTP; // 1001 N Price The distance in pips from the entry price at which the Take Profit will be triggered.
    double AbsoluteSL;// 1002 N Price The absolute price at which Stop Loss will be triggered.
    double RelativeSL; // 1003 N Price The distance in pips from the entry price at which the Stop Loss will be triggered.
    double TrailingSL;// 1004 N Boolean Indicates if Stop Loss is trailing.N = The Stop Loss is not trailing, Y = The Stop Loss is trailing.
    
    // Indicated trigger method of the Stop Loss
    // 1 = The Stop Loss will be triggered by the trade side. 
    // 2 = The stop loss will be triggered by the opposite side(Ask for Buy positions and by Bid for Sell positions), 
    // 3 = Stop Loss will be triggered after two consecutive ticks according to the trade side. 
    // 4 = Stop Loss will be triggered after two consecutive ticks according to the opposite side(second Ask tick for Buy positions and second Bid tick for Sell positions). 
    uint8_t TriggerMethodSL;// 1005 N Integer . 

    // Indicates if Stop Loss is guaranteed.
    // N = The Stop Loss is not guarranteed, 
    // Y = The Stop Loss is guarranteed.
    uint8_t GuaranteedSL; // 1006  N Boolean 


} order_notice_t;


// 持仓明细.
typedef struct
{
    char symbolcode[16];  // 合约代码.
    // t_order_direction direction;  // 持仓方向.
    t_position_type position_type; // 仓位类型 今仓 昨仓.
    double price;  // 成交价格..
    double volume;
    char orderid[16];  // 报单编号.
    std::string cccccc;  // 成交编号.
    time_t time; // @3 :UInt32 = 0; # 时间.
    int timemsec; // @4 :UInt16 = 0; # 毫秒.

} t_position;


// 合约持仓汇总.
typedef struct
{
    std::string code1;
	// 合约代码.
    // 今多仓数量.
    // 今多可平量.
    // 昨多仓数量.
    // 昨多可平量.
    // 今空仓数量.
    // 今空可平量.
    // 昨空仓数量.
    // 昨空可平量.
    // 持仓量.
    // 多仓量.
    // 可平多仓量.
    // 空仓量.
    // 可平空仓量.
    //
    //
    //
    //
    //t_order_direction direction;  // 持仓方向.
    t_position_type position_type; // 仓位类型 今仓 昨仓.
    double price;  // 成交价格..
    std::string code3;  // 报单编号.
    std::string cccccc;  // 成交编号.
    std::string vol;  // 成交数量.
    std::string time;  // 成交时间.

} t_position_all2;

enum openmode
{
    OPENMODE_NEW = 0,
    OPENMODE_APPEND = 1,
};

// 市场代码.
enum EnumMarket {
    MARKET_NONE = 0, // @0;
    MARKET_SSE = 1, // @1; # 上海证券.
    MARKET_SZSE = 2, // @2; # 深圳证券.
    MARKET_CFFEX = 3, // @3; # CFFEX，即中国金融期货交易所.
    MARKET_DCE = 4, // @4; # 大商所DCE.
    MARKET_CZCE = 5, // @5; # 郑商所CZCE.
    MARKET_SHFE = 6, // @6; # 上期所SHFE.
    MARKET_FOREXMT5 = 7, // @7; # MT5外汇.
    MARKET_IBKR = 8, // @8; # 盈透证券.
};

enum EnumTickType {
    TICKTYPE_NONE = 0, //
    TICKTYPE_TYPEQUOTE = 1, // # 十档行情.
    TICKTYPE_TYPEORDERQUEUE = 2, // # 委托队列.
    TICKTYPE_TYPETRANSACTION = 3, // # 分笔成交.
    TICKTYPE_TYPEORDER = 4, // # 分笔委托.
};

// 逐笔委托.
typedef struct {
    int aaa;
}t_order;

// 委托队列.
typedef struct {
    int side; //  @0 :SideOrderQueue; #	买卖方向	1 = BIDQUEUE 2 = OFFERQUEUE.
    int nopricelevel; //  @1 :UInt32 = 0; # 买卖盘价位数量	10档行情，不足时补空.
    double price; //  @2 :Float64 = 0; #	委托价格.
    int numorders; //  @3 :UInt32 = 0; # 最优价位上的委托笔数.
    int ordervolume; //  @4 :List(UInt32); # 委托量01 OrderVolume(N)表示在此委托价格上, 按时间优先的原则进入委托队列第N位的委托量.
    int noorders; //  @5 :UInt32 = 0; # 发布的委托笔数.
    int volume; //  @6 :UInt32 = 0; #	委托量.
}t_orderqueue;

// 逐笔成交.
typedef struct {
    double price; // @0 :Float64 = 0; # 成交价格.
    int volume; // @1 :UInt32 = 0; # 成交数量.
    int type; // @2 :TradeType; # 成交类型.
} t_transaction;

typedef struct
{
    double lastprice; // @0 :Float64 = 0; # 最新价.
    double presettlementprice; // @1 :Float64 = 0; # 上次结算价.
    double precloseprice; // @2 :Float64 = 0; # 昨收盘.
    double preopeninterest; // @3 :Float64 = 0; # 昨持仓量.
    double openprice; // @4 :Float64 = 0; # 今开盘.
    double highestprice; // @5 :Float64 = 0; # 最高价.
    double lowestprice; // @6 :Float64 = 0; # 最低价.
    double averageprice; // @7 :Float64 = 0; # 当日均价.
    double volume; // @8 :UInt32 = 0; # 数量.
    double turnover; // @9 :Float64 = 0; # 成交金额.
    double openinterest; // @10 :Float64 = 0; # 持仓量.
    double closeprice; // @11 :Float64 = 0; # 今收盘.
    double settlementprice; // @12 :Float64 = 0; # 本次结算价.
    double upperlimitprice; // @13 :Float64 = 0; # 涨停板价.
    double lowerlimitprice; // @14 :Float64 = 0; # 跌停板价.

    uint8_t level; // 行情档数 一般期货外汇一档 股票有五档或十档 上海黄金可能有一档或五档.

    double BuyPrice[10];              // 申买价1,2,3
    double BuyVolume[10];            // 申买量1,2,3
    double SellPrice[10];              // 申卖价1,2,3
    double SellVolume[10];             // 申卖量1,2,3

} t_qoute;

typedef struct {
    char symbolcode[16]; // @0 :Text = ""; #合约代码.
    char symbolname[32]; // @1 :Text = ""; #合约名称.
    EnumTickType type; // @2 :TickType;
    time_t time; // @3 :UInt32 = 0; # 行情时间.
    int timemsec; // @4 :UInt16 = 0; # 行情毫秒.
    EnumMarket market; // @5 :Market; # 市场代码.

    union
    {
        t_qoute qoute; // 十档行情.
        t_order order; // 逐笔委托.
        t_orderqueue orderqueue; // 委托队列.
        t_transaction transaction; // 逐笔成交.
    };
} t_tick;



#ifdef TRADEONE_BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
//#define DLL_EXPORT __declspec(dllimport)
#define DLL_EXPORT
#endif

#endif // __TRADEONE_MAIN_DEF_H__

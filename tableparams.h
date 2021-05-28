#pragma once

enum ColumnType {
    SignalCh0,
    MeasureDeltaCh0,
    //MeasurePpmCh0,
    MeasureCh0,
    SignalCh1,
    MeasureDeltaCh1,
    //MeasurePpmCh1,
    MeasureCh1,
    ColumnCount

};

enum { RowCount = 16 };

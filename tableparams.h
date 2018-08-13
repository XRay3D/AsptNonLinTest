#ifndef TABLEPARAMS_H
#define TABLEPARAMS_H

enum ColumnType {
    SignalCh0,
    MeasureDeltaCh0,
    //MeasurePpmCh0,
    MeasureCh0,
    SignalCh1,
    MeasureDeltaCh1,
    //MeasurePpmCh1,
    MeasureCh1
};

enum {
    RowCount = 16,
    ColumnCount = MeasureCh1 + 1
};

#endif // TABLEPARAMS_H

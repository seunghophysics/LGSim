import pandas as pd
from pandas.io.common import EmptyDataError

def read_data(file):
    lgcol = ['initialKE', 'path_length', 'E_dep']
    pmtcol = ['charge']
    try:
        if "pmt" in file:
            df = pd.read_csv(file, comment='#', names=pmtcol, header=None)
        elif "lg" in file:
            df = pd.read_csv(file, comment='#', names=lgcol, header=None)
    except EmptyDataError:
        df = pd.DataFrame()
    return df

# merge pmt and lg (pmt + lg for same thread)

lg_csv = [None] * 8
pmt_csv = [None] * 8
thread_csv = [None] * 8

for no in range(0, 8):
    thread = "t" + str(no)
    lg_csv[no] = read_data("data_nt_lg_{}.csv".format(thread))
    pmt_csv[no] = read_data("data_nt_pmt_{}.csv".format(thread))
    thread_csv[no] = lg_csv[no].join(pmt_csv[no])
    
# merge thread-wise (t0 + t1 + ... + t7)

result = pd.concat(thread_csv, ignore_index=True)
result.to_csv("data.csv", index=False, float_format='%3e')
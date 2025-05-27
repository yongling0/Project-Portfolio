import pandas as pd
import sqlite3

conn = sqlite3.connect("C:/Users/A/Dropbox/PC/Desktop/bonus_database/HW2_database.db")
cursor = conn.cursor()

cursor.execute('''
CREATE TABLE IF NOT EXISTS TeamPitchingStats (
    TStatsID INTEGER PRIMARY KEY AUTOINCREMENT,
    SeasonID INTEGER,
    TeamID INTEGER,
    W INTEGER,
    L INTEGER,
    ERA REAL,
    G INTEGER,
    GS INTEGER,
    CG INTEGER,
    SHO INTEGER,
    SV INTEGER,
    SVO INTEGER,
    IP REAL,
    H INTEGER,
    R INTEGER,
    ER INTEGER,
    HR INTEGER,
    HB INTEGER,
    BB INTEGER,
    SO INTEGER,
    WHIP REAL,
    AVG REAL,
    TBF INTEGER,
    NP INTEGER,
    PIP REAL,
    QS INTEGER,
    GF INTEGER,
    HLD INTEGER,
    IBB INTEGER,
    WP INTEGER,
    BK INTEGER,
    GDP INTEGER,
    GOAO REAL,
    SO9 REAL,
    BB9 REAL,
    KBB REAL,
    FOREIGN KEY (SeasonID) REFERENCES Season(SeasonID),
    FOREIGN KEY (TeamID) REFERENCES Team(TeamID)
)
''')
conn.commit()

csv_path = "C:/Users/A/Dropbox/PC/Desktop/1111410_HW2/team_pitching.csv"
df = pd.read_csv(csv_path)

df_data = df.iloc[:, 3:].copy()

def get_season_id(year):
    cursor.execute("SELECT SeasonID FROM Season WHERE SeasonYear = ?", (year,))
    result = cursor.fetchone()
    return result[0] if result else None

def get_team_id(team_name):
    cursor.execute("SELECT TeamID FROM Team WHERE TeamName = ?", (team_name,))
    result = cursor.fetchone()
    return result[0] if result else None

df_data['SeasonID'] = df['Year'].apply(get_season_id)
df_data['TeamID'] = df['TEAM'].apply(get_team_id)

fixed_columns = ['SeasonID', 'TeamID']
other_columns = [col for col in df_data.columns if col not in fixed_columns]
df_data = df_data[fixed_columns + other_columns]

df_data.reset_index(drop=True, inplace=True)
df_data.insert(0, 'TStatsID', df_data.index + 1)  

if df_data['SeasonID'].isnull().any():
    print("有找不到對應的 SeasonID")
if df_data['TeamID'].isnull().any():
    print("有找不到對應的 TeamID")

df_data.to_sql('TeamPitchingStats', conn, if_exists='replace', index=False)

conn.close()

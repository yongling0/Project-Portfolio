import sqlite3
import pandas as pd
import random

conn = sqlite3.connect("C:/Users/A/Dropbox/PC/Desktop/bonus_database/HW2_database.db")
cursor = conn.cursor()

cursor.execute('''
CREATE TABLE IF NOT EXISTS PitchingStats (
    StatsID INTEGER PRIMARY KEY,
    SeasonID INTEGER,
    PlayerID INTEGER,
    TeamID TEXT,
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
    BABIP REAL,
    SB INTEGER,
    CS INTEGER,
    PK INTEGER,
    FOREIGN KEY (SeasonID) REFERENCES Season(SeasonID),
    FOREIGN KEY (PlayerID) REFERENCES Player(PlayerID)
)
''')
conn.commit()

df = pd.read_csv("C:/Users/A/Dropbox/PC/Desktop/1111410_HW2/player_pitching.csv")

stats_data = df.iloc[:, 3:].copy()

def get_season_id(year):
    cursor.execute("SELECT SeasonID FROM Season WHERE SeasonYear = ?", (year,))
    result = cursor.fetchone()
    return result[0] if result else None

def get_player_id(name):
    if not isinstance(name, str):
        return None  
    
    cleaned_name = name.replace(" ", "")  
    cursor.execute("SELECT PlayerID FROM Player WHERE REPLACE(PlayerName, ' ', '') = ?", (cleaned_name,))
    result = cursor.fetchone()
    return result[0] if result else None


abbrev_to_fullname = {
    "AZ": "Arizona Diamondbacks",
    "ATL": "Atlanta Braves",
    "BAL": "Baltimore Orioles",
    "BOS": "Boston Red Sox",
    "CHC": "Chicago Cubs",
    "CWS": "Chicago White Sox",
    "CIN": "Cincinnati Reds",
    "CLE": "Cleveland Guardians",  
    "COL": "Colorado Rockies",
    "DET": "Detroit Tigers",
    "HOU": "Houston Astros",
    "KC": "Kansas City Royals",
    "LAA": "Los Angeles Angels",
    "LAD": "Los Angeles Dodgers",
    "MIA": "Miami Marlins",
    "MIL": "Milwaukee Brewers",
    "MIN": "Minnesota Twins",
    "NYM": "New York Mets",
    "NYY": "New York Yankees",
    "OAK": "Oakland Athletics",
    "PHI": "Philadelphia Phillies",
    "PIT": "Pittsburgh Pirates",
    "SD": "San Diego Padres",
    "SF": "San Francisco Giants",
    "SEA": "Seattle Mariners",
    "STL": "St. Louis Cardinals",
    "TB": "Tampa Bay Rays",  
    "TEX": "Texas Rangers",
    "TOR": "Toronto Blue Jays",
    "WSH": "Washington Nationals",
    "MON": "Montreal Expos",
    "ANA": "Anaheim Angels",
    "FLA": "Florida Marlins",
    "LA": "Los Angeles Dodgers",
}

# === 根據縮寫取得 TeamID ===
def get_team_id(abbrev):
    fullname = abbrev_to_fullname.get(abbrev)
    if not fullname:
        return None

    cursor.execute("SELECT TeamID FROM Team WHERE TeamName = ?", (fullname,))
    result = cursor.fetchone()
    return result[0] if result else None


stats_data['TeamID'] = df.iloc[:, 2].apply(get_team_id)  


stats_data['SeasonID'] = df.iloc[:, 0].apply(get_season_id)   
stats_data['PlayerID'] = df.iloc[:, 1].apply(get_player_id)   

stats_data.insert(0, "StatsID", range(1, len(stats_data) + 1))


fixed_columns = ['StatsID', 'SeasonID', 'PlayerID', 'TeamID']
other_columns = [col for col in stats_data.columns if col not in fixed_columns]
stats_data = stats_data[fixed_columns + other_columns]



invalid_rows = stats_data[stats_data['SeasonID'].isnull() | stats_data['PlayerID'].isnull()]
if not invalid_rows.empty:
    print("⚠️ 以下資料無法對應到 SeasonID 或 PlayerID，未寫入：")
    print(invalid_rows[['StatsID', 'SeasonID', 'PlayerID', 'TeamID']])
else:

    stats_data.to_sql('PitchingStats', conn, if_exists='replace', index=False)
    print("✅ 所有資料成功寫入 Stats 資料表。")

conn.close()

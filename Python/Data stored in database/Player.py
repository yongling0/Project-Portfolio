import pandas as pd
import sqlite3

df = pd.read_csv("C:/Users/A/Dropbox/PC/Downloads/pitching_player_id.csv")

df = df[['Player ID', 'PLAYER']]

df['PlayerName'] = df['PLAYER'].apply(
    lambda x: x.split(';')[1].strip() if isinstance(x, str) and ';' in x else ''
)

df = df[df['PlayerName'] != '']

name_conflicts = df.groupby('Player ID')['PlayerName'].nunique().reset_index()
name_conflicts = name_conflicts[name_conflicts['PlayerName'] > 1]

conflicting_rows = df[df['Player ID'].isin(name_conflicts['Player ID'])]

print(conflicting_rows.sort_values(by='Player ID'))

unique_player_ids = df['Player ID'].drop_duplicates().reset_index(drop=True)
player_id_map = {pid: idx + 1 for idx, pid in enumerate(unique_player_ids)}
df['PlayerID'] = df['Player ID'].map(player_id_map)

final_df = df[['PlayerID', 'PlayerName']].drop_duplicates()


conn = sqlite3.connect(r"C:\Users\A\Dropbox\PC\Desktop\bonus_database\HW2_database.db")
cursor = conn.cursor()

cursor.execute('''
    CREATE TABLE IF NOT EXISTS Player (
        PlayerID INTEGER PRIMARY KEY,
        PlayerName TEXT
    );
''')

final_df.to_sql('Player', conn, if_exists='replace', index=False)

conn.commit()
conn.close()

print("✅ Player 資料表已成功建立，符合指定邏輯。")
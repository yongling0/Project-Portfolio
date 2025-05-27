import pandas as pd
import sqlite3

df = pd.read_csv("C:/Users/A/Dropbox/PC/Desktop/1111410_HW2/player_pitching.csv")

unique_years = sorted(df['Year'].unique())

season_df = pd.DataFrame({
    'SeasonID': range(1, len(unique_years) + 1),
    'SeasonYear': unique_years
})

conn = sqlite3.connect("C:/Users/A/Dropbox/PC/Desktop/bonus_database/HW2_database.db")
cursor = conn.cursor()

cursor.execute('''
    CREATE TABLE IF NOT EXISTS Season (
        SeasonID INTEGER PRIMARY KEY,
        SeasonYear INTEGER
    );
''')

cursor.execute("DELETE FROM Season")

season_df.to_sql('Season', conn, if_exists='append', index=False)


conn.commit()
conn.close()

print("已成功寫入唯一的 SeasonYear 到 Season 資料表中。")

import pandas as pd
import sqlite3

df = pd.read_csv("C:/Users/A/Dropbox/PC/Desktop/1111410_HW2/team_pitching.csv")

unique_teams = df[['TEAM', 'LEAGUE']].drop_duplicates().reset_index(drop=True)

grouped_teams = [
    {"Cleveland Indians", "Cleveland Guardians"},
    {"Tampa Bay Rays", "Tampa Bay Devil Rays"}
]

team_name_to_group_key = {}
for group in grouped_teams:
    sorted_names = sorted(group)
    group_key = sorted_names[0]
    for name in group:
        team_name_to_group_key[name] = group_key

all_group_keys = set(team_name_to_group_key.get(name, name) for name in unique_teams['TEAM'])

sorted_group_keys = sorted(all_group_keys)
group_key_to_team_id = {name: idx + 1 for idx, name in enumerate(sorted_group_keys)}

final_team_ids = []
for _, row in unique_teams.iterrows():
    team_name = row['TEAM']
    group_key = team_name_to_group_key.get(team_name, team_name)
    team_id = group_key_to_team_id[group_key]
    final_team_ids.append(team_id)

unique_teams['TeamID'] = final_team_ids

unique_teams = unique_teams[['TeamID', 'TEAM', 'LEAGUE']]
unique_teams.columns = ['TeamID', 'TeamName', 'League']

conn = sqlite3.connect("C:/Users/A/Dropbox/PC/Desktop/bonus_database/HW2_database.db")
cursor = conn.cursor()

cursor.execute('''
    CREATE TABLE IF NOT EXISTS Team (
        TeamID INTEGER,
        TeamName TEXT,
        League TEXT,
        PRIMARY KEY (TeamID, TeamName)
    );
''')

unique_teams = unique_teams.sort_values('TeamID').reset_index(drop=True)
unique_teams.to_sql('Team', conn, if_exists='replace', index=False)


conn.commit()
conn.close()

print("已成功依字母順序分配 TeamID，並將 Cleveland 與 Tampa Bay 兩組隊名設為共用 TeamID。")

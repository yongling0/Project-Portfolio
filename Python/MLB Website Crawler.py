import csv
import time
from time import sleep
import multiprocessing
from bs4 import BeautifulSoup
from selenium import webdriver
from multiprocessing import Pool
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from webdriver_manager.chrome import ChromeDriverManager
from selenium.common.exceptions import TimeoutException
from selenium.common.exceptions import NoSuchElementException

def setup_driver():
    options = Options()
    # 添加一些防止被檢測為爬蟲的選項
    options.add_argument("--disable-blink-features=AutomationControlled")
    options.add_argument("user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36")
    driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)    
    return driver

def accept_cookies(driver):
    try:
        cookie_button = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.XPATH, "//button[text()='OK']"))
        )
        cookie_button.click()
    except Exception as e:
        print("No cookie button found or already accepted:", e)

def scrape_headers(driver, headers, data_type):
    html = driver.page_source
    soup = BeautifulSoup(html, 'html.parser')
    # 使用更通用的選擇器，找到包含 'bui-table' 類的表格
    table = soup.find('table', class_=lambda x: x and 'bui-table' in x)
    
    if table is None:
        print("❌ Table not found! Check your selector or wait for JavaScript content.")
        print("Page title:", driver.title)
        print("Current URL:", driver.current_url)
        print(soup.prettify()[:1000])  # 只印出前1000字符，避免輸出過多
        return
    
    rows = table.find_all('tr')
    if data_type == 'Standard':
        for th in rows[0].find_all('th'):
            abbr = th.find('abbr')
            if abbr:
                headers.append(abbr.text.strip())
            else:
                headers.append(th.text.strip())
    else:
        counter = 0 
        for th in rows[0].find_all('th'):
            abbr = th.find('abbr')
            if abbr:
                if counter >= 2:
                    headers.append(abbr.text.strip())
                counter += 1  
            else:
                headers.append(th.text.strip())

def scrape_data(driver, headers, data_list, data_type, year):
    try:
        # 修改等待條件，使用更通用的選擇器
        WebDriverWait(driver, 15).until(
            EC.presence_of_all_elements_located((By.CSS_SELECTOR, "table tr:nth-child(2)"))
        )
    except TimeoutException:
        print("Wait timeout")

    html = driver.page_source
    soup = BeautifulSoup(html, 'html.parser')
    # 使用更通用的選擇器
    table = soup.find('table', class_=lambda x: x and 'bui-table' in x)
    if table:
        rows = table.find_all('tr')[1:]  
        for row in rows:
            cells = row.find_all('td')
            a_tag = row.find('th').find('a')
            if a_tag and 'aria-label' in a_tag.attrs:
                full_name = a_tag['aria-label'].strip()
                team = cells[0].text.strip()  
                unique_key = f"{full_name} ({team}, {year})"  
            else:
                full_name = 'N/A'
                team = cells[0].text.strip()
                unique_key = f"Unknown ({team}, {year})"
            
            cols = [td.text.strip() for td in cells]

            if data_type == 'Standard':
                data_dict = dict(zip(headers, [full_name, team] + cols[1:]))  
                data_list[unique_key] = data_dict  
            elif data_type == 'Expanded' and unique_key in data_list:
                current_data = data_list[unique_key]
                expanded_headers = headers[len(current_data):]  
                expanded_data = dict(zip(expanded_headers, cols[1:]))
                data_list[unique_key].update(expanded_data) 
    else:
        print("Can't find table")
        print("HTML content:")
        print(soup.prettify()[:1000])  # 只印出前1000字符

def expand_table(driver):
    try:
        expand_buttons = WebDriverWait(driver, 10).until(
            EC.visibility_of_all_elements_located((By.XPATH, "//button[@aria-label='Expanded']"))
        )
        for button in expand_buttons:
            button.click()
            WebDriverWait(driver, 10).until(
                EC.presence_of_element_located((By.XPATH, "//button[@aria-label='Expanded']"))
            )
    except Exception as e:
        print("No expand button found or expansion failed:", e)

def standard_table(driver):
    try:
        expand_buttons = WebDriverWait(driver, 10).until(
            EC.visibility_of_all_elements_located((By.XPATH, "//button[@aria-label='Standard']"))
        )
        for button in expand_buttons:
            button.click()
            WebDriverWait(driver, 10).until(
                EC.presence_of_element_located((By.XPATH, "//button[@aria-label='Standard']"))
            )
    except Exception as e:
        print("No expand button found or expansion failed:", e)

def load_page(driver, year, page_number):
    if page_number == 1:
        url = f"https://www.mlb.com/stats/pitching/{year}?sortState=asc"
        
    else:
        url = f"https://www.mlb.com/stats/pitching/{year}?sortState=asc&page={page_number}"

    driver.get(url)
    try:
        # 調整等待條件
        WebDriverWait(driver, 15).until(
            EC.visibility_of_element_located((By.XPATH, '//table//th'))
        )
        return True
    except TimeoutException:
        print(f"Failed to load page {page_number} for year {year}")
        return False


def save_data_to_csv(all_data, headers, type):
    if type == 'Player':
        filename = 'player.csv'
    else:
        filename = 'team_pitching.csv'
    with open(filename, 'w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        headers.insert(0, 'Year')
        writer.writerow(headers)
        for year_index, year_data in enumerate(all_data, start=2003):
            for unique_key, stats in year_data.items():
                name_team_year = unique_key.split(' (')  
                full_name = name_team_year[0]  
                team_year = name_team_year[1].rstrip(')').split(', ') 
                team = team_year[0]
                row = [year_index, full_name, team] + [stats.get(header, 'N/A') for header in headers[3:]]
                writer.writerow(row)

def process_year_player(year, headers):
    data = {}
    driver = setup_driver()
    page_number = 1
    try:
        url = f'https://www.mlb.com/stats/pitching/{year}?sortState=asc'
        driver.get(url)
        accept_cookies(driver)

        while True:
            if not load_page(driver, year, page_number):
                print(f"Could not load page {page_number}")
                break
                
            # 增加等待時間確保頁面完全加載
            sleep(5)
            
            standard_table(driver)
            sleep(3.5)
            scrape_data(driver, headers, data, 'Standard', year)
            
            expand_table(driver)
            sleep(3)
            scrape_data(driver, headers, data, 'Expanded', year)
            
            try:
                next_button = driver.find_element(By.XPATH, '//button[@aria-label="next page button"]')
                if next_button.is_enabled():
                    next_button.click()
                    page_number += 1
                    sleep(3)  # 等待新頁面加載
                else:
                    print(f"Next button disabled after page {page_number}")
                    break
            except NoSuchElementException:
                print(f"No more pages to load after page {page_number}")
                break
    finally:
        driver.quit()
    return (year, data)

def process_year_team(year, headers):
    data = {}
    driver = setup_driver()
    try:
        url = f'https://www.mlb.com/stats/team/pitching/{year}?sortState=asc'
        driver.get(url)
        accept_cookies(driver)
           
        # 增加等待時間確保頁面完全加載
        sleep(5)
            
        standard_table(driver)
        sleep(3.5)
        scrape_data(driver, headers, data, 'Standard', year)
            
        expand_table(driver)
        sleep(3)
        scrape_data(driver, headers, data, 'Expanded', year)
    finally:
        driver.quit()
    return (year, data)

def get_headers(headers_player, type):
    driver = setup_driver()
    if type == 'Player':
        url = f'https://www.mlb.com/stats/pitching/2023?sortState=asc'
    else:
        url = f'https://www.mlb.com/stats/team/pitching/2023?sortState=asc'
    driver.get(url)
    accept_cookies(driver)   
    # 增加等待時間
    sleep(3)   
    scrape_headers(driver, headers_player, 'Standard')
    expand_table(driver)
    sleep(2)
    scrape_headers(driver, headers_player, 'Expanded')
    sleep(1)
    driver.quit()

def main():
    start_time = time.time()
    all_data_player = []
    headers_player = []
    get_headers(headers_player,'Player')
    # 可以調整年份範圍
    with Pool(processes=4) as pool:  # 減少並行進程數以避免被封鎖
        year_ranges = [(year, headers_player) for year in range(2003, 2024)]
        results = pool.starmap(process_year_player, year_ranges)   

    results.sort()  
    for year, data in results:
        all_data_player.append(data)

    save_data_to_csv(all_data_player, headers_player, 'Player')

    all_data_team = []
    headers_team = []
    get_headers(headers_team,'Team')
    with Pool(processes=4) as pool:  # 減少並行進程數以避免被封鎖
        year_ranges = [(year, headers_team) for year in range(2003, 2024)]
        results = pool.starmap(process_year_team, year_ranges)
    results.sort()  
    for year, data in results:
        all_data_team.append(data)
    save_data_to_csv(all_data_team, headers_team, 'Team')

    end_time = time.time()
    print(f"Program completed in {end_time - start_time:.2f} seconds.")

if __name__ == "__main__":
    main()

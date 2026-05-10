import streamlit as st
import pandas as pd
import numpy as np
import plotly.express as px

st.set_page_config(page_title="NOAA Data Dashboard", layout="wide")

@st.cache_data
def load_data():
    try:
        df = pd.read_csv('vhi_clean_data.csv')
    except FileNotFoundError:
        st.error("Файл 'vhi_clean_data.csv' не знайдено!")
        return pd.DataFrame()
    df = df.rename(columns={'Province_Name': 'Region'})

    if 'VCI' not in df.columns:
        df['VCI'] = df['VHI'] * np.random.uniform(0.9, 1.1, len(df))
        df['VCI'] = df['VCI'].clip(0, 100)
    if 'TCI' not in df.columns:
        df['TCI'] = (df['VHI'] - 0.5 * df['VCI']) * 2
        df['TCI'] = df['TCI'].clip(0, 100)
    df = df.dropna(subset=['VCI', 'TCI', 'VHI', 'Region'])
    return df

df = load_data()
if df.empty:
    st.stop()

def reset_filters():
    for key in ['metric', 'region', 'weeks', 'years', 'sort_asc', 'sort_desc']:
        if key in st.session_state:
            del st.session_state[key]

col_controls, col_content = st.columns([1, 3])
with col_controls:
    st.header("Фільтри")
    st.button("Скинути фільтри", on_click=reset_filters)
    st.markdown("---")
    metric = st.selectbox("Оберіть показник:", ['VHI', 'VCI', 'TCI'], key='metric')
    unique_regions = sorted([r for r in df['Region'].unique() if pd.notna(r) and isinstance(r, str)])
    region = st.selectbox("Оберіть область:", unique_regions, key='region')
    min_week, max_week = int(df['Week'].min()), int(df['Week'].max())
    weeks = st.slider("Інтервал тижнів:", min_week, max_week, (min_week, max_week), key='weeks')
    min_year, max_year = int(df['Year'].min()), int(df['Year'].max())
    years = st.slider("Інтервал років:", min_year, max_year, (min_year, max_year), key='years')
    st.markdown("---")
    st.subheader("Сортування таблиці")
    sort_asc = st.checkbox(f"Сортувати за зростанням {metric}", key='sort_asc')
    sort_desc = st.checkbox(f"Сортувати за спаданням {metric}", key='sort_desc')

    if sort_asc and sort_desc:
        st.warning("Обидва типи сортування вимкнено.")
        apply_sort = None
    elif sort_asc:
        apply_sort = 'asc'
    elif sort_desc:
        apply_sort = 'desc'
    else:
        apply_sort = None

with col_content:
    st.title("Аналіз вегетаційних індексів")
    filtered_df = df[
        (df['Region'] == region) &
        (df['Year'] >= years[0]) & (df['Year'] <= years[1]) &
        (df['Week'] >= weeks[0]) & (df['Week'] <= weeks[1])
        ].copy()

    if apply_sort == 'asc':
        filtered_df = filtered_df.sort_values(by=metric, ascending=True)
    elif apply_sort == 'desc':
        filtered_df = filtered_df.sort_values(by=metric, ascending=False)
    if 'Province_ID' in filtered_df.columns:
        filtered_df = filtered_df.drop(columns=['Province_ID'])

    tab1, tab2, tab3 = st.tabs(["🗃 Таблиця даних", "📈 Графік часового ряду", "📊 Порівняння областей"])
    with tab1:
        st.subheader(f"Дані: {region} ({years[0]}-{years[1]})")
        st.dataframe(filtered_df, use_container_width=True)

    with tab2:
        st.subheader(f"Динаміка {metric} ({region})")
        if not filtered_df.empty:
            plot_df = filtered_df.sort_values(by=['Year', 'Week'])
            plot_df['Period'] = plot_df['Year'].astype(str) + "-W" + plot_df['Week'].astype(str).str.zfill(2)
            fig1 = px.line(plot_df, x='Period', y=metric)
            fig1.update_xaxes(title_text="Рік - Тиждень (W)")
            st.plotly_chart(fig1, use_container_width=True)
        else:
            st.info("Немає даних.")

    with tab3:
        st.subheader(f"Порівняння {metric} між областями")
        comp_df = df[
            (df['Year'] >= years[0]) & (df['Year'] <= years[1]) &
            (df['Week'] >= weeks[0]) & (df['Week'] <= weeks[1])
            ]
        if not comp_df.empty:
            avg_comp_df = comp_df.groupby('Region')[metric].mean().reset_index()
            avg_comp_df = avg_comp_df.sort_values(by=metric, ascending=False)

            avg_comp_df['Color'] = np.where(avg_comp_df['Region'] == region, 'Обрана область', 'Інші області')

            fig2 = px.bar(avg_comp_df, x='Region', y=metric, color='Color',
                          color_discrete_map={'Обрана область': '#ef553b', 'Інші області': '#636efa'})
            st.plotly_chart(fig2, use_container_width=True)
        else:
            st.info("Немає даних.")
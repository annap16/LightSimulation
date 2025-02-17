
![Screenshot](https://private-user-images.githubusercontent.com/160714456/413877535-5c4e221b-fe72-40d0-9b15-3da8fa9b5a5f.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3Mzk3OTkwMzEsIm5iZiI6MTczOTc5ODczMSwicGF0aCI6Ii8xNjA3MTQ0NTYvNDEzODc3NTM1LTVjNGUyMjFiLWZlNzItNDBkMC05YjE1LTNkYThmYTliNWE1Zi5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjUwMjE3JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI1MDIxN1QxMzI1MzFaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT0yM2Y1YWEyMmNkNGM2MDVlOTRiMTFlODA2MTA3MDI0MTMyZDVjOWQwNWI0YWFlNzNhYjQ0NGEzNTc1ZGU0ZTg4JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.EFiAXoAal1PvnoPdQdSBvF4wuNeQfdKuhpDnZViSXJA)
# Modelowanie oświetlenia - Powierzchnia Beziera 3-go Stopnia

Repozytorium zawiera projekt, którego celem jest modelowanie oświetlenia i nakładanie tekstur na powierzchni 3D opartej na powierzchni Beziera 3-go stopnia. 
Projekt wczytuje dane z pliku tekstowego przy starcie aplikacji i pozwala na interakcję z powierzchnią w celu obserwacji wpływu różnych parametrów oświetlenia oraz wyświetlania tekstur na tych powierzchniach.

## Opis Projektu

### Wejście
Powierzchnia Beziera 3-go stopnia jest wczytywana z pliku tekstowego zawierającego 16 punktów kontrolnych (po 3 liczby rzeczywiste: x, y, z dla każdego punktu).

### Właściwości
- **Triangulacja powierzchni** – Generowanie siatki trójkątów na podstawie powierzchni Beziera.
- **Obroty powierzchni** – Powierzchnia jest obracana wokół osi Z i X, a użytkownik może modyfikować kąty obrotu za pomocą suwaków (kąty α od -45° do 45° oraz β od 0° do 90°).
- **Wypełnianie trójkątów** – Możliwość rysowania samej siatki trójkątów lub wypełniania ich odpowiednimi kolorami na podstawie modelu oświetlenia.
- **Dokładność triangulacji** – Suwak umożliwiający kontrolowanie dokładności triangulacji powierzchni.
- **Interpolacja kolorów** – Obliczanie koloru wypełnienia w trójkątach za pomocą modelu oświetlenia Lamberta (składowe rozproszone i zwierciadlane).
- **Ruch światła** – Animacja ruchu źródła światła po okręgu na płaszczyźnie (z opcją zatrzymania).
- **Reflektory** – Możliwość włączania/wyłączania reflektorów w 3 podstawowych kolorach (czerwony, zielony, niebieski) oraz zmieniania ich parametru mL za pomocą suwaka (ostatni suwak).
- **Tekstura** – Możliwość wybrania tekstury dla obiektu, która będzie zastosowana do powierzchni Beziera.
- **NormalMap** - Możliwość wczytania mapy wektorów normalnych (NormalMap) oraz modyfikowania wektora normalnego na podstawie tej mapy.

### Algorytmy
- **Obroty** – Obrót powierzchni wokół osi Z i X za pomocą odpowiednich macierzy obrotu.
- **Interpolacja barycentryczna** – Kolor wypełnienia punktu w trójkącie obliczany na podstawie współrzędnych barycentrycznych.
- **Obliczanie koloru** – Kolor obiektu jest obliczany za pomocą wzoru oświetlenia Lamberta i uwzględnia refleksję w zależności od kąta między wektorem normalnym a źródłem światła.

### Oświetlenie
- W aplikacji zastosowano **Model Lamberta** zgodnie z poniższym wzorem:
  - Wzór na oświetlenie:  
    `I = kd * IL * IO * cos(∠(N, L)) + ks * IL * IO * cos^m(∠(V, R))`
    Gdzie:
    - `kd` i `ks` to współczynniki dla składowych rozproszonych i zwierciadlanych.
    - `IL` i `IO` to kolory światła i obiektu.
    - `N` to wektor normalny.
    - `L` to wersor do źródła światła.
    - `V` to wersor widza (zwykle [0, 0, 1]).
    - `R` to wektor odbicia światła.
    - `m` to współczynnik opisujący jak bardzo trójkąt jest zwierciadlany.

# Lighting Modeling - Degree 3 Bézier Surface

This repository contains a project aimed at modeling lighting and applying textures to a 3D surface based on a degree 3 Bézier surface. The project loads data from a text file at the start of the application and allows interaction with the surface to observe the effects of different lighting parameters and display textures on the surfaces.

## Project Description

### Input
The degree 3 Bézier surface is loaded from a text file containing 16 control points (each with 3 real numbers: x, y, z for each point).

### Features
- **Surface Triangulation** – Generation of a triangle mesh based on the Bézier surface.
- **Surface Rotation** – The surface is rotated around the Z and X axes, and the user can modify the rotation angles using sliders (angles α from -45° to 45° and β from 0° to 90°).
- **Triangle Filling** – Option to draw only the triangle mesh or fill the triangles with appropriate colors based on the lighting model.
- **Triangulation Accuracy** – A slider to control the accuracy of the surface triangulation.
- **Color Interpolation** – Calculating the fill color in triangles using Lambert's lighting model (diffuse and specular components).
- **Light Movement** – Animation of the light source moving in a circular path on the plane (with an option to stop the animation).
- **Spotlights** – Ability to toggle spotlights on/off in 3 primary colors (red, green, blue) and adjust their parameter mL using a slider (the last slider).
- **Texture** – Ability to select a texture for the object, which will be applied to the Bézier surface.
- **NormalMap** – Ability to load a normal map (NormalMap) and modify the normal vector based on this map.

### Algorithms
- **Rotation** – Rotation of the surface around the Z and X axes using appropriate rotation matrices.
- **Barycentric Interpolation** – The color of the fill in a triangle is calculated based on barycentric coordinates.
- **Color Calculation** – The object's color is computed using Lambert's lighting formula, considering the reflection based on the angle between the normal vector and the light source.

### Lighting
The **Lambertian model** is applied in the application using the following formula:
- Lighting formula:  
  `I = kd * IL * IO * cos(∠(N, L)) + ks * IL * IO * cos^m(∠(V, R))`
  Where:
  - `kd` and `ks` are the coefficients for the diffuse and specular components.
  - `IL` and `IO` are the light and object colors.
  - `N` is the normal vector.
  - `L` is the light direction vector.
  - `V` is the viewer direction vector (usually [0, 0, 1]).
  - `R` is the light reflection vector.
  - `m` is the coefficient describing how specular the triangle is.



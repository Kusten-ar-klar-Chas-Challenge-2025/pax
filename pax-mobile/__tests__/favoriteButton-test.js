import React from "react";
import { render } from "@testing-library/react-native";
import FavoriteButton from "../components/favoriteButton";

jest.mock("../theme/ThemeContext", () => ({
    useTheme: () => ({
        theme: {
            textSecondary: "blue",
        },
    }),
}));

test("renderar rätt text när isFavorite är false", () => {
    const { getByText } = render(<FavoriteButton isFavorite={false} onToggle={() => { }} />);
    expect(getByText("☆ Lägg till favorit")).toBeTruthy();
});

test("renderar rätt text när isFavorite är true", () => {
    const { getByText } = render(<FavoriteButton isFavorite={true} onToggle={() => { }} />);
    expect(getByText("★ Ta bort som favorit")).toBeTruthy();
});

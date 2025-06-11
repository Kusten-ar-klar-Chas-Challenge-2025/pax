import React from "react";
import { render, waitFor } from "@testing-library/react-native";
import FetchRoom from "../components/FetchRoom";

// Mocka useTheme
jest.mock("../theme/ThemeContext", () => ({
    useTheme: () => ({
        theme: {
            accent: "blue",
            textPrimary: "black",
            textSecondary: "gray",
            card: "white",
            background: "white",
        },
    }),
}));

// Mocka fetch som returnerar en tom lista
global.fetch = jest.fn(() =>
    Promise.resolve({
        json: () => Promise.resolve([]),
    })
);

describe("FetchRoom", () => {
    it('visar "Inga lediga rum hittades" om listan är tom', async () => {
        const mockSetAllRooms = jest.fn();

        const { getByText } = render(
            <FetchRoom
                favorites={{}}
                setFavorites={() => { }}
                setAllRooms={mockSetAllRooms}
            />
        );

        await waitFor(() =>
            expect(getByText("Inga lediga rum hittades.")).toBeTruthy()
        );
    });
});

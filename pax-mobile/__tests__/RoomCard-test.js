import React from "react";
import { render, fireEvent } from "@testing-library/react-native";
import RoomCard from "../components/RoomCard";

jest.mock("../theme/ThemeContext", () => ({
    useTheme: () => ({
        theme: {
            card: "#ffffff",
            textPrimary: "#000000",
            textSecondary: "#007AFF",
            text: "#333333",
        },
    }),
}));

test("startar timer när pausa knapp klickas", () => {
    const { getByText, queryByText } = render(<RoomCard />);

    expect(queryByText(/Ditt rum är pausat/i)).toBeNull();

    fireEvent.press(getByText("Pausa bokning"));

    expect(getByText(/Ditt rum är pausat/i)).toBeTruthy();
});

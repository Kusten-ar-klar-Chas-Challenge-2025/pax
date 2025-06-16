import { render } from "@testing-library/react-native";
import ButtonComponent from "../components/ButtonComponent";

jest.mock("../theme/ThemeContext", () => ({
  useTheme: () => ({
    theme: {
      textSecondary: "blue",
    },
  }),
}));

test("Visar titeln korrekt", () => {
  const { getByText } = render(
    <ButtonComponent title="Logga in" onPress={() => {}} />
  );
  expect(getByText("Logga in")).toBeTruthy();
});

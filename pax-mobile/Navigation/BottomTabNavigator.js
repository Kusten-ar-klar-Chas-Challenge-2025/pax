import React from "react";
import { View } from "react-native";
import { createBottomTabNavigator } from "@react-navigation/bottom-tabs";
import { MaterialCommunityIcons } from "@expo/vector-icons";
import Icon from "react-native-vector-icons/FontAwesome5";
import HomeScreen from "../screens/HomeScreen";
import Booking from "../screens/Booking";
import BookingButton from "../components/BookingButton";
import Contact from "../screens/Contact";
import LoginScreen from "../screens/LoginScreen";
import DataChart from "../screens/Datachart";
import { useTheme } from "../theme/ThemeContext";
import { useAuth } from "../context/AuthContext";

const Tab = createBottomTabNavigator();

export default function BottomTabNavigator() {
  const { customTheme } = useTheme();
  const { token, logout } = useAuth();

  return (
    <Tab.Navigator
      screenOptions={({ route }) => ({
        headerShown: false,
        tabBarShowLabel: false,
        tabBarActiveTintColor: "rgb(228, 255, 183)",
        tabBarInactiveTintColor: "rgb(36, 82, 75)",
        tabBarIcon: ({ focused, color, size }) => {
          let iconName;

          if (route.name === "Login") iconName = "sign-in-alt";
          else if (route.name === "Home") iconName = "home";
          else if (route.name === "Logout") iconName = "sign-out-alt";
          else if (route.name === "DataChart") iconName = "chart-line";
          else if (route.name === "Booking") return null;
          else if (route.name === "Contact") {
            return (
              <View
                style={{
                  backgroundColor: "#84ca6f",
                  borderRadius: 25,
                  padding: 10,
                  alignItems: "center",
                  justifyContent: "center",
                }}
              >
                <MaterialCommunityIcons name="account-box" size={size} color={color} />
              </View>
            );
          }

          return (
            <View
              style={{
                backgroundColor: "#84ca6f",
                borderRadius: 25,
                padding: 10,
                alignItems: "center",
                justifyContent: "center",
              }}
            >
              <Icon name={iconName} size={size} color={color} />
            </View>
          );
        },
        tabBarStyle: {
          backgroundColor: customTheme.colors.tabBar,
          paddingBottom: 0,
          paddingTop: 10,
          height: 60,
          borderTopWidth: 0,
          elevation: 0,
        },
      })}
    >
      <Tab.Screen name="Home" component={HomeScreen} />
      <Tab.Screen name="DataChart" component={DataChart} />
      <Tab.Screen
        name="Booking"
        component={Booking}
        options={{
          tabBarButton: (props) => <BookingButton {...props} />,
        }}
      />

      <Tab.Screen name="Contact" component={Contact} />

      {!token && <Tab.Screen name="Login" component={LoginScreen} />}

      {token && (
        <Tab.Screen
          name="Logout"
          listeners={({ navigation }) => ({
            tabPress: (e) => {
              e.preventDefault();
              logout(null);
            },
          })}
          options={{
            tabBarLabel: "Logga ut",
            tabBarIcon: ({ color, size }) => (
              <View
                style={{
                  backgroundColor: "#84ca6f",
                  borderRadius: 25,
                  padding: 10,
                  alignItems: "center",
                  justifyContent: "center",
                }}
              >
                <Icon name="sign-out-alt" size={size} color={color} />
              </View>
            ),
          }}
        >
          {() => null}
        </Tab.Screen>
      )}
    </Tab.Navigator>
  );
}

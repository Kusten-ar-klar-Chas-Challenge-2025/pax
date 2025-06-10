import React from "react";
import { NavigationContainer } from "@react-navigation/native";
import { View, ActivityIndicator } from "react-native";
import BottomTabNavigator from "./Navigation/BottomTabNavigator";
import { ThemeProvider } from "./theme/ThemeContext";
import { UserProvider } from "./context/UserContext";
import { AuthProvider } from "./context/AuthContext";
import { useAuth } from "./context/AuthContext";
import AuthNavigator from './Navigation/AuthNavigator'

export default function App() {

  return (
    <ThemeProvider>
      <UserProvider>
        <AuthProvider>
          <NavigationContainer>
            <AppContent />
          </NavigationContainer>
        </AuthProvider>
      </UserProvider>
    </ThemeProvider>
  );
}

function AppContent () {
  const { token, isLoading } = useAuth();

  if (isLoading) {
    return (
      <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
        <ActivityIndicator size="large" color="#84ca6f" />
        </View>
    );
  }
    
  return token ? <BottomTabNavigator /> : <AuthNavigator />
}

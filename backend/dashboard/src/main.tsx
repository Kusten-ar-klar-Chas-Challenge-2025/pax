import { createRoot } from "react-dom/client";
import "./index.css";
import { BrowserRouter, Routes, Route, Navigate, useLocation } from "react-router-dom";
import Auth from "./pages/Auth";
import Stats from "./pages/Stats";
import Users from "./pages/Users";
import Navbar from "./components/Navbar";
import Dashboard from "./pages/Dashboard";
import ProtectedRoute from "./components/ProctectedRoute";

// Komponent för att hantera Navbar beroende på vilken väg vi är på
const AppWithNavbar = () => {
  const location = useLocation();

  return (
    <>
      {/* Rendera Navbar om vi inte är på /login */}
      {location.pathname !== "/login" && <Navbar />}

      <Routes>
        <Route path="/" element={<Navigate to="/login" replace />} />
        <Route path="/login" element={<Auth />} />
        <Route
          path="/dashboard"
          element={
            <ProtectedRoute allowedRoles={["admin"]}>
              <Dashboard />
            </ProtectedRoute>
          }
        />
        <Route path="/stats" element={<Stats />} />
        <Route path="/users" element={<Users />} />
      </Routes>
    </>
  );
};

const rootElement = document.getElementById("root");

if (!rootElement) {
  throw new Error("Root element not found");
}

createRoot(rootElement).render(
  <BrowserRouter>
    <AppWithNavbar />
  </BrowserRouter>
);
